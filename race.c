#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include <sys/time.h>

#include "abdrive.h"
#include "simpletext.h"
#include "simpletools.h"
#include "ping.h"
#include "movement.h"
#include "ir_stuff.h"
#include "map.h"
#include "list.h"
#include "pathing.h"

#ifdef BUILDING_IN_SIMULATOR
#include "simulator.h"
#endif

const float MOVE_TIME = 2100;
const float FIRST_MOVE_TIME = 2450;
// const float TURN_STRENGTH = 60;

// North is 0
int direction = 0;
int location = -4;
int lastTime = 0;
// struct timeval tv;

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


// typedef struct path_node{
//     int loc;
//     struct path_node *left;
//     struct path_node *right;
// } pathNode;


void forward(float time){
    moveFor(60, time);
    int loc = updateLocation(&location,direction);
    visitedCells[loc] = 1;
    detectWalls(distances,direction);
    updateMap(distances,location,map);
    prettyPrintMap(map);
}

// void getMeasurements(){
//     printf("right: %d\n", getRightDist());
//     printf("left: %d\n", getLeftDist());
//     printf("front: %d\n", ping_cm(8));
//     printf("----------------\n");
// }

// get the possible cells robot can move to given current location.
// Checks the wall cell in 'map' and only treats '0' cells as valid to move into.
// int getNextSteps(int locations[], int map[7][7], int location){
//     int i = 0;
//     if(location < 12 && !map[(location/4)*2+1][(location%4)*2]){
//         map[(location/4)*2+1][(location%4)*2] = 1;
//         locations[i] = location + 4;
//         i++;
//     }
//     if(location % 4 < 3 && !map[(location/4)*2][(location%4)*2+1]){
//         map[(location/4)*2][(location%4)*2+1] = 1;
//         locations[i] = location + 1;
//         i++;
//     }
//     if(location > 3 && !map[(location/4)*2-1][(location%4)*2]){
//         map[(location/4)*2-1][(location%4)*2] = 1;
//         locations[i] = location - 4;
//         i++;
//     }
//     if(location % 4 && !map[(location/4)*2][(location%4)*2-1]){
//         map[(location/4)*2][(location%4)*2-1] = 1;
//         locations[i] = location - 1;
//         i++;
//     }

//     return i;
// }

// void findNextNode(ListNode *node){
//     int locations[3];
//     int numSteps = getNextSteps(locations, map, node->loc);
//     // printf("path: %i\n", node->loc);
//     for(int i = 0; i < numSteps; i++){
//         if(pathCell != 15){
//             map[(location/4)*2][(location%4)*2] = 1;
//             ListNode* newNode;
//             newNode = malloc(sizeof(ListNode));
//             newNode->loc = locations[i];
//             node->next = newNode;
//             pathCell = locations[i];
//             findNextNode(newNode);
//         }
//     }
// }

// ListNode* genPaths(){
//     ListNode* head;
//     head = malloc(sizeof(ListNode));
//     head->loc = 0;
//     findNextNode(head);
//     return head;
// }

void wallFollow(){
    /// enter maze:
    forward(FIRST_MOVE_TIME);

    while(1){
        printf("Ir right: %i\n", getRightDist());
        
        // Determines end of circuit
        if(!location && direction){
            moveFor(60, MOVE_TIME);
            turn(2,&direction);
            location = -4;
            break;
        }
        //if you can turn left: do it
        if (getLeftDist() == 20){
            turn(-1,&direction);
        }

        // else (if you can't turn left), if you can keep going straight: go straight
        else if (ping_cm(8) > 30){
            // do nothing
        }
        // else (if you can't reach either of previous two steps), if you can turn right: do it
        else if (getRightDist() == 20){
            turn(1,&direction);
        }

        // if you reached a dead end, turn back by turning 180
        else{
            turn(2,&direction);
        }
        forward(MOVE_TIME);
    }
}


int main(int argc, const char* argv[])
{
    #ifdef BUILDING_IN_SIMULATOR
        simulator_startNewSmokeTrail();
    #endif
    // printMap(map);
    initMap(map);

    for(int i = 0; i < 16; i++){
        visitedCells[i] = 0;
        path[i] = -2;
    }
    visitedCells[0] = 1;

    int mapNum = 1;

    if(mapNum == 1){
        path[0] = 0;
        path[1] = 1;
        path[2] = 5;
        path[3] = 9;
        path[4] = 10;
        path[5] = 14;
        path[6] = 15;
    }else if(mapNum == 2){
        path[0] = 0;
        path[1] = 4;
        path[2] = 8;
        path[3] = 12;
        path[4] = 13;
        path[5] = 14;
        path[6] = 15;
    }else if(mapNum == 3){
        path[0] = 0;
        path[1] = 4;
        path[2] = 5;
        path[3] = 9;
        path[4] = 10;
        path[5] = 11;
        path[6] = 15;
    }else if(mapNum == 4){
        path[0] = 0;
        path[1] = 4;
        path[2] = 8;
        path[3] = 12;
        path[4] = 13;
        path[5] = 9;
        path[6] = 5;
        path[7] = 1;
        path[8] = 2;
        path[9] = 3;
        path[10] = 7;
        path[11] = 11;
        path[12] = 10;
        path[13] = 14;
        path[14] = 15;
    }

    // gettimeofday(&tv, NULL);
    // int time = tv.tv_sec * 1e6 + tv.tv_usec;

    // for(int i = 0; i < 20; i++){
    //     printf("speed: %i\n", getSpeed(&lastTime,&tv));
    //     pause(200);
    // }


    drive_speed(60,60);
    pause(MOVE_TIME/2 - MOVE_TIME + FIRST_MOVE_TIME);

    movePath(path);

    // wallFollow();

    // getTargetLocation(visitedCells);

    // genPaths();
    // printList(genPaths());


    return 0;
}
