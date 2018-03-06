#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include <sys/time.h>

#include "abdrive.h"
#include "simpletext.h"
#include "simpletools.h"
#include "ping.h"
#include "turning.h"
#include "movement.h"
#include "ir_stuff.h"
#include "map.h"
#include "list.h"

#ifdef BUILDING_IN_SIMULATOR
#include "simulator.h"
#endif

const float MOVE_DISTANCE = 25.8;
const int TURN_SPEED = 9;

int direction = 0;
int location = -4;

int map[7][7];
/* 
    0 - Unvisited
    1 - Visited (for pathing)
    2 - wall
    3 - null cell

    MAP 1
    {
        {0,0,0,0,0,0,0},
        {2,3,0,3,2,3,0},
        {0,0,0,2,0,2,0},
        {0,3,0,3,0,3,0},
        {0,2,0,0,0,2,0},
        {0,3,2,3,0,3,2},
        {0,0,0,0,0,0,0}
    };

    MAP 2
    {
        {0,2,0,0,0,0,0},
        {0,3,0,3,2,3,0},
        {0,0,0,0,0,2,0},
        {0,3,2,3,0,3,0},
        {0,0,0,2,0,0,0},
        {0,3,0,3,0,3,2},
        {0,0,0,0,0,0,0}
    };

    {
        {0,0,0,0,0,0,0},
        {0,3,0,3,0,3,0},
        {0,0,0,0,0,0,0},
        {0,3,0,3,0,3,0},
        {0,0,0,0,0,0,0},
        {0,3,0,3,0,3,0},
        {0,0,0,2,0,0,0}
    };
*/
// int map[7][7] = {
//             {0,0,0,0,0,0,0},
//             {0,3,0,3,0,3,0},
//             {0,0,0,0,0,0,0},
//             {0,3,0,3,0,3,0},
//             {0,0,0,0,0,0,0},
//             {0,3,0,3,0,3,0},
//             {0,0,0,2,0,0,0}
//         };
int distances[4];
int visitedCells[16];
int path[16];
int pathCell;

double angle;
double pi;


typedef struct path_node{
    int loc;
    struct path_node *left;
    struct path_node *right;
} pathNode;


void forward(float distance){
    drive_goto(move(distance), move(distance));
    int loc = updateLocation(&location,direction);
    visitedCells[loc] = 1;
    detectWalls(distances,direction);
    updateMap(distances,location,map);
    printMap(map);
}

void getMeasurements(){
    printf("right: %d\n", getRightDist());
    printf("left: %d\n", getLeftDist());
    printf("front: %d\n", ping_cm(8));
    printf("----------------\n");
}

// get the possible cells robot can move to given current location.
// Checks the wall cell in 'map' and only treats '0' cells as valid to move into.
int getNextSteps(int locations[], int map[7][7], int location){
    int i = 0;
    if(location < 12 && !map[(location/4)*2+1][(location%4)*2]){
        map[(location/4)*2+1][(location%4)*2] = 1;
        locations[i] = location + 4;
        i++;
    }
    if(location % 4 < 3 && !map[(location/4)*2][(location%4)*2+1]){
        map[(location/4)*2][(location%4)*2+1] = 1;
        locations[i] = location + 1;
        i++;
    }
    if(location > 3 && !map[(location/4)*2-1][(location%4)*2]){
        map[(location/4)*2-1][(location%4)*2] = 1;
        locations[i] = location - 4;
        i++;
    }
    if(location % 4 && !map[(location/4)*2][(location%4)*2-1]){
        map[(location/4)*2][(location%4)*2-1] = 1;
        locations[i] = location - 1;
        i++;
    }

    return i;
}

void findNextNode(ListNode *node){
    int locations[3];
    int numSteps = getNextSteps(locations, map, node->loc);
    // printf("path: %i\n", node->loc);
    for(int i = 0; i < numSteps; i++){
        if(pathCell != 15){
            map[(location/4)*2][(location%4)*2] = 1;
            ListNode* newNode;
            newNode = malloc(sizeof(ListNode));
            newNode->loc = locations[i];
            node->next = newNode;
            pathCell = locations[i];
            findNextNode(newNode);
        }
    }
}

ListNode* genPaths(){
    ListNode* head;
    head = malloc(sizeof(ListNode));
    head->loc = 0;
    findNextNode(head);
    return head;
}

void wallFollow(){
    /// enter maze:
    forward(30.5);
    
    while(1){

        angle = getAngle();
        
        //if you can turn left: do it
        if (getLeftDist() == 20){
            //turnThroughAngle(90);
            pi = 2.9495;
            while (getAngle() > angle - pi/2){
                drive_speed(-TURN_SPEED,TURN_SPEED);
            }
            updateDirection(&direction,-1);
            if(!location && direction){
                pause(100);
                drive_speed(0,0);
                pause(100);
                drive_goto(move(MOVE_DISTANCE), move(MOVE_DISTANCE));
                location = -4;
                break;
            }
        }

        // else (if you can't turn left), if you can keep going straight: go straight
        else if (ping_cm(8) > 30){
        }
        // else (if you can't reach either of previous two steps), if you can turn right: do it
        else if (getRightDist() == 20){
            //turnThroughAngle(-90);
            // turning 90 + right
            pi = 2.9495;
            while (getAngle() < angle + pi/2){
                drive_speed(TURN_SPEED,-TURN_SPEED);
            }
            updateDirection(&direction,1);
        }

        // if you reached a dead end, turn back by turning 180
        else{
            //turnThroughAngle(180);
            pi = 2.92;
            // turning 180 - left
            while (getAngle() > angle - pi){
                drive_speed(-20,20);
            }
            updateDirection(&direction,2);
        }
        pause(100);
        drive_speed(0,0);
        pause(100);
        forward(MOVE_DISTANCE);

    }
}


int main(int argc, const char* argv[])
{
#ifdef BUILDING_IN_SIMULATOR
    simulator_startNewSmokeTrail();
#endif
    printMap(map);
    initMap(map);

    for(int i = 0; i < 16; i++){
        visitedCells[i] = 0;
        path[i] = -1;
    }
    visitedCells[0] = 1;

    wallFollow();

    // getTargetLocation(visitedCells);

    // genPaths();
    // printList(genPaths());


    return 0;
}
