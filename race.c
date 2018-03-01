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

#ifdef BUILDING_IN_SIMULATOR
#include "simulator.h"
#endif

const float speedUp = 2;

double driveSpeed = 30 * speedUp;
double theta, targetTheta;

int direction = 0;
int location = -1;

// int map[24];
int map[7][7];

void printMap(){
    for(int i = 6; i >= 0; i--){
        for(int j = 0; j < 7; j++){
            printf("%i ",map[i][j]);
        }
        printf("\n");
    }
}

void see(){
    int leftDist = getLeftDist();
    int rightDist = getRightDist();
    int frontDist = ping_cm(8);

    // see if there's a wall around
    // leftDist = leftDist > 10 ? 1 : 0;
    // rightDist = rightDist < 10 ? 1 : 0;
    // frontDist = frontDist < 30 ? 1 : 0;

    if(leftDist){
        printf("wall on left\n");
    }

    printf("%i\n", leftDist);
    printf("%i\n", rightDist);
    printf("%i\n", frontDist);
}


int main(int argc, const char* argv[])
{
#ifdef BUILDING_IN_SIMULATOR
    simulator_startNewSmokeTrail();
#endif

    // init map
    for(int i = 0; i < 7; i++){
        for(int j = 0; j < 7; j++){
            map[i][j] = i%2==1 && j%2==1 ? 1 : 0;
        }
    }

    // hard code first move into square 1.
    drive_goto(140,140);

    // while(ping_cm(8) > 20){
    //     // see();
    //     // pause(100);
    // }
    see();

    printMap();

    // while(ping_cm(8) > 10){
    //     int irLeft = getLeftDist(1,0);
    //     int irRight = getRightDist(1,0);
    // }

    return 0;
}
