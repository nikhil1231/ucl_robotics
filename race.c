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

double angle;
double pi;

// move x small boxes:
float move(float x){
    return x*100/21;
}

void forward(){
    drive_goto(move(25.8), move(25.8));
    see(distances,direction);
    updateMap(distances,location,map);
    prettyPrintMap(map);
}

void getMeasurements(){
    printf("right: %d\n", getRightDist());
    printf("left: %d\n", getLeftDist());
    printf("front: %d\n", ping_cm(8));
    printf("----------------\n");
}

// clockwise = positive; radians
// 90 degrees = pi/4

void wallFollow(){
    /// enter maze:
    drive_goto(move(30.5), move(30.5));
    
        getMeasurements();
    
        // start going through:
        int counter = 0;
        while(counter < 21){
    
            angle = getAngle();
            
            //if you can turn left: do it
            if (getLeftDist(3,0) == 20){
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
            else if (getRightDist(3,0) == 20){
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

    wallFollow();

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
