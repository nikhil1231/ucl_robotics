#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "abdrive.h"
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

// North is 0
int direction = 0;
int location = -4;

int map[7][7];
int visitedCorners[3];

int distances[4];
int path[16];

void forward(float time){
    moveFor(60, time);
    updateLocation(&location,direction,visitedCorners);
    detectWalls(distances,direction);
    updateMap(distances,location,map);
    // prettyPrintMap(map);
}

int hasVisitedAllCorners(){
    int i = 1;
    for(int j = 0; j < 3; j++){
        if(!visitedCorners[j]) i = 0;
    }
    return i;
}

void wallFollow(){
    /// enter maze:
    forward(FIRST_MOVE_TIME);

    while(1){
        // Determines end of circuit
        if(!location && direction && hasVisitedAllCorners()){
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

int main(int argc, const char* argv[])
{
    #ifdef BUILDING_IN_SIMULATOR
        simulator_startNewSmokeTrail();
    #endif
    initMap(map);

    memset(path, -2, sizeof(path));

    int mapNum = 2;

    if(mapNum == 1){
        path[0] = 0;
        path[1] = 1;
        path[2] = 5;
        path[3] = 9;
        path[4] = 10;
        path[5] = 14;
        path[6] = 15;

        // path[0] = 0;
        // path[1] = 1;
        // path[2] = 5;
        // path[3] = 4;
        // path[4] = 8;
        // path[5] = 12;
        // path[6] = 13;
        // path[7] = 14;
        // path[8] = 15;
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
    }else if(mapNum == 5){
        path[0] = 0;
        path[1] = 1;
        path[2] = 2;
        path[3] = 3;
        path[4] = 7;
        path[5] = 6;
        path[6] = 5;
        path[7] = 4;
        path[8] = 8;
        path[9] = 9;
        path[10] = 10;
        path[11] = 14;
        path[12] = 15;
    }else if(mapNum == 8){
        path[0] = 0;
        path[1] = 1;
        path[2] = 2;
        path[3] = 6;
        path[4] = 10;
        path[5] = 14;
        path[6] = 15;
    }

    // wallFollow();

    // buildPathTree(map,path);

    drive_speed(MAX_SPEED,MAX_SPEED);
    pause(FINAL_MOVE_TIME / 3);

    movePath(path);

    return 0;
}
