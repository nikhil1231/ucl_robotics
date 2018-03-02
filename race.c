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

const int IR_LIMIT = 20;
const int US_LIMIT = 30;
const int MOVE_DISTANCE = 122;

int direction = 0;
int location = 0;

int map[7][7];
int distances[4];


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

void wallFollow(){

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
}


void printMap(){
    for(int i = 6; i >= 0; i--){
        for(int j = 0; j < 7; j++){
            int c = map[i][j];
            if(c == 2){
                printf(". ");
            }else if(c == 1){
                printf("X ");
            }else{
                printf("0 ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

void prettyPrintMap(){
    printf("\n");
    int height = 4;
    int width = 7;
    for(int i = 0; i < 4*height+1; i++){
        if(i%4==0){
            for(int j = 0; j < 4; j++){
                for(int k = 0; k < width; k++){
                    if((!j&&!k) || i == height*4 || !i || map[(4-i/4)*2-1][j*2]){
                        printf("*");
                    }else{
                        printf(" ");
                    }
                }
            }
        }else{
            for(int j = 0; j < 4; j++){
                for(int k = 0; k < width; k++){
                    if((!j && !k) || (!k && map[(3-i/4)*2][j*2-1])){
                        printf("*");
                    }else{
                        printf(" ");
                    }
                }
            }
        }
        printf("*\n");
    }
}

void updateMap(int dists[]){
    // north
    if(location < 12 && dists[1] > 0) map[(location/4)*2+1][(location%4)*2] = dists[1];
    // east
    if(location % 4 < 3 && dists[2] > 0) map[(location/4)*2][(location%4)*2+1] = dists[2];
    // south
    if(location > 3 && dists[3] > 0) map[(location/4)*2-1][(location%4)*2] = dists[3];
    // west
    if(location % 4 && dists[0] > 0) map[(location/4)*2][(location%4)*2-1] = dists[0];
}

void see(int dists[]){
    // Treat distances like compass, not relative to the robot.
    int* leftDist = &dists[(direction)%4];
    int* frontDist = &dists[(direction+1)%4];
    int* rightDist = &dists[(direction+2)%4];
    dists[(direction+3)%4] = -1;

    *leftDist = getLeftDist();
    *frontDist = ping_cm(8);
    *rightDist = getRightDist();

    // see if there's a wall around
    *leftDist = *leftDist < IR_LIMIT ? 1 : 0;
    *rightDist = *rightDist < IR_LIMIT ? 1 : 0;
    *frontDist = *frontDist < US_LIMIT ? 1 : 0;

    // printf("%i\n", *leftDist);
    // for(int i = 0; i < 4; i++){
    //     printf("%i\n", dists[i]);
    // }
}

void forward(){
    drive_goto(MOVE_DISTANCE,MOVE_DISTANCE);
    see(distances);
    updateMap(distances);
    prettyPrintMap();
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

    wallFollow();

    turn(1,&direction);

    see(distances);
    updateMap(distances);
    printMap();

    location += 1;
    forward();

    turn(0,&direction);

    location = 5;
    forward();
    location = 9;
    forward();

    turn(1,&direction);

    location = 10;
    forward();

    turn(1,&direction);

    location = 6;
    forward();


    return 0;
}
