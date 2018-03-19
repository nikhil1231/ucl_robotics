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
#include "queue.h"
#include "pathing.h"

#ifdef BUILDING_IN_SIMULATOR
#include "simulator.h"
#endif

const float MAX_SPEED = 128;
const float MOVE_TIME = 2100;
const float FIRST_MOVE_TIME = 2450;
const float FINAL_MOVE_TIME = 1500;
// const float TURN_STRENGTH = 60;

// North is 0
int direction = 0;
int location = -4;
int lastTime = 0;

// int map[7][7];
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
int map[7][7] = {
            {0,0,0,0,0,0,0},
            {2,3,0,3,2,3,0},
            {0,0,0,2,0,2,0},
            {0,3,0,3,0,3,0},
            {0,2,0,0,0,2,0},
            {0,3,2,3,0,3,2},
            {0,0,0,0,0,0,0}
        };
int distances[4];
int visitedCells[16];
int path[16];
int pathCell;

double angle;
double pi;


void forward(float time){
    moveFor(60, time);
    int loc = updateLocation(&location,direction);
    visitedCells[loc] = 1;
    detectWalls(distances,direction);
    updateMap(distances,location,map);
    prettyPrintMap(map);
}

void wallFollow(){
    /// enter maze:
    forward(FIRST_MOVE_TIME);

    while(1){
        // Determines end of circuit
        if(!location && direction){
            if(direction == 3) turn(-1,&direction);
            moveFor(60, FINAL_MOVE_TIME);
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
    return (row >= 0) && (row < 7) && (col >= 0) && (col < 7);
}

void buildPathTree(int map[][7], int path[]){
    struct Point src = {0,0};
    int rowNum[] = {-1, 0, 0, 1};
    int colNum[] = {0, -1, 1, 0};
    int index = 0;

    bool visited[7][7];
    memset(visited, false, sizeof(visited));

    visited[src.x][src.y] = true;

    struct queueNode s = {src, -1};
    push(s);

    while (true){
        struct queueNode curr = get(index);
        struct Point pt = curr.pt;

        if (pt.x == 6 && pt.y == 6){
            getPath(curr ,path);
            return;
        }

        for (int i = 0; i < 4; i++)
        {
            int row = pt.x + rowNum[i];
            int col = pt.y + colNum[i];
            
            if (isValid(row, col) && (map[row][col] == 0) && !visited[row][col])
            {
                visited[row][col] = true;
                struct queueNode Adjcell = { {row, col}, index };
                push(Adjcell);
            }
        }
        
        index ++;
    }
}

int main(int argc, const char* argv[])
{
    #ifdef BUILDING_IN_SIMULATOR
        simulator_startNewSmokeTrail();
    #endif
    // printMap(map);
    initMap(map);

    memset(path, -2, sizeof(path));

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

    wallFollow();

    buildPathTree(map,path);

    drive_speed(MAX_SPEED,MAX_SPEED);
    pause(FINAL_MOVE_TIME / 3);

    movePath(path);

    // printf ("\nShortest path is: %d", dist);


    return 0;
}
