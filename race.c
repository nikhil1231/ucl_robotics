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

int left, right;
double widthRobot = 105.8; //mm
double angle;
double pi;

// move x small boxes:
float move(float x){
    return x*100/21;
}

void forward(){
    drive_goto(move(25.8), move(25.8));
}

void getMeasurements(){
    printf("right: %d\n", getRightDist(3,0));
    printf("left: %d\n", getLeftDist(3,0));
    printf("front: %d\n", ping_cm(8));
    printf("----------------\n");
}

// clockwise = positive; radians
// 90 degrees = pi/4
double getAngle(){
    drive_getTicks (&left, &right);
    return (left-right)*3.25/widthRobot;
  }


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

    /// enter maze:
    drive_goto(move(30.5), move(30.5));

    getMeasurements();

    // start going through:
    int counter = 0;
    while(counter < 21){

        angle = getAngle();
        
        //if you can turn left: do it
        if (getLeftDist(3,0) > 13){
            //turnThroughAngle(90);
            pi = 2.7;
            // turning 90 - left
            while (getAngle() > angle - pi/2){
                drive_speed(-20,20);
            }
            pause(100);
            drive_speed(0,0);
            pause(100);
            forward();
        }

        // else (if you can't turn left), if you can keep going straight: go straight
        else if (ping_cm(8) > 22){
            forward();
        }
        // else (if you can't reach either of previous two steps), if you can turn right: do it
        else if (getRightDist(3,0) > 13){
            //turnThroughAngle(-90);
            // turning 90 + right
            pi = 2.7;
            while (getAngle() < angle + pi/2){
                drive_speed(20,-20);
            }
            pause(100);
            drive_speed(0,0);
            pause(100);
            forward();
        }
        // if you reached a dead end, turn back by turning 180
        else{
            //turnThroughAngle(180);
            pi = 2.92;
            // turning 180 - left
            while (getAngle() > angle - pi){
                drive_speed(-20,20);
            }
            pause(100);
            drive_speed(0,0);
            pause(100);
            forward();
        }
    
        getMeasurements();
        counter++;
    }

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
