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

#ifdef BUILDING_IN_SIMULATOR
#include "simulator.h"
#endif


const int MOVE_DISTANCE = 122;

int direction = 0;
int location = 0;

int map[7][7];
int distances[4];
int path[16];


void forward(){
    drive_goto(MOVE_DISTANCE,MOVE_DISTANCE);
    see(distances,direction);
    updateMap(distances,location,map);
    printMap(map);
}

int findEnd(int pos){
    // if(pos < 12) map[(pos/4)*2+1][(pos%4)*2] = dists[1];
    // if(pos % 4 < 3 && dists[2] > 0) map[(pos/4)*2][(pos%4)*2+1] = dists[2];
    // if(pos > 3 && dists[3] > 0) map[(pos/4)*2-1][(pos%4)*2] = dists[3];
    // if(pos % 4 && dists[0] > 0) map[(pos/4)*2][(pos%4)*2-1] = dists[0];
}


int main(int argc, const char* argv[])
{
#ifdef BUILDING_IN_SIMULATOR
    simulator_startNewSmokeTrail();
#endif

    // init map
    for(int i = 0; i < 7; i++){
        for(int j = 0; j < 7; j++){
            map[i][j] = i%2==1 && j%2==1 ? 2 : 0;
        }
    }

    // hard code first move into square 1.
    drive_goto(140,140);

    see(distances,direction);
    updateMap(distances,location,map);
    printMap(map);

    location = 4;
    forward();

    turn(1,&direction);

    location = 5;
    forward();
    location = 6;
    forward();

    turn(0,&direction);

    location = 10;
    forward();

    turn(1,&direction);

    location = 11;
    forward();




    return 0;
}
