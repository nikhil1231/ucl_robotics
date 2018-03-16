#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include <sys/time.h>
#include <stdbool.h>
#include <string.h>

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

//to store matrix cell cordinates
struct Point{
    int x;
    int y;
};

// a data structure for queue used in BFS
struct queueNode{
    struct Point pt;  // The cordinates of a cell
    int dist;  // cell's distance of from the source
};

// implement queue:
#define MAX 20000

struct queueNode q[MAX];
int front = 0;
int rear = -1;
int itemCount = 0;

struct queueNode peep() {
   return q[front];
}

bool isEmpty() {
   return itemCount == 0;
}

bool isFull() {
   return itemCount == MAX;
}

int size() {
   return itemCount;
}  

void push(struct queueNode data) {

   if(!isFull()) {
	
      if(rear == MAX-1) {
         rear = -1;            
      }       

      q[++rear] = data;
      itemCount++;
   }
}

struct queueNode pop() {
   struct queueNode data = q[front++];
	
   if(front == MAX) {
      front = 0;
   }
	
   itemCount--;
   return data;  
}

// end of queue implementation

int map[7][7] = 
{
    {0,0,0,0,0,0,0},
    {2,3,0,3,2,3,0},
    {0,0,0,2,0,2,0},
    {0,3,0,3,0,3,0},
    {0,2,0,0,0,2,0},
    {0,3,2,3,0,3,2},
    {0,0,0,0,0,0,0}
};
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

bool isSafe(int map[7][7], int visited[7][7], int x, int y){
    if (map[x][y] == 2 || map[x][y] == 3 || visited[x][y]){
        return false;
    }

    return true;
}

bool isValid(int row, int col)
{
    // return true if row number and column number
    // is in range
    return (row >= 0) && (row < 7) &&
           (col >= 0) && (col < 7);
}

// These arrays are used to get row and column
// numbers of 4 neighbours of a given cell
int rowNum[] = {-1, 0, 0, 1};
int colNum[] = {0, -1, 1, 0};

int BFS(int map[][7]){
    struct Point src = {0,0};
    struct Point dest = {6,6};

    bool visited[7][7];
    memset(visited, false, sizeof(visited));

    // Mark the source cell as visited
    visited[src.x][src.y] = true;

    struct queueNode s = {src, 0};
    push(s);

    while (!isEmpty()){
        struct queueNode curr = peep();
        struct Point pt = curr.pt;

        // If we have reached the destination cell, we are done
        if (pt.x == dest.x && pt.y == dest.y)
            {return curr.dist;}

        // Otherwise dequeue the front cell in the queue
        // and enqueue its adjacent cells
        pop();

        for (int i = 0; i < 4; i++)
        {
            int row = pt.x + rowNum[i];
            int col = pt.y + colNum[i];
             
            // if adjacent cell is valid, has path and
            // not visited yet, enqueue it.
            if (isValid(row, col) && (map[row][col] == 0) && !visited[row][col])
            {
                // mark cell as visited and enqueue it
                visited[row][col] = true;
                struct queueNode Adjcell = { {row, col}, curr.dist + 1 };
                push(Adjcell);
            }
        }
        
    }

    return -1;
    
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

    int mapNum = 4;

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


    // drive_speed(60,60);
    // pause(MOVE_TIME/2 - MOVE_TIME + FIRST_MOVE_TIME);

    // movePath(path);

    wallFollow();

    // getTargetLocation(visitedCells);

    // genPaths();
    // printList(genPaths());


    return 0;
}
