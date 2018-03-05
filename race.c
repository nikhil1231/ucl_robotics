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

const float MOVE_DISTANCE = 25.8;
const int TURN_SPEED = 9;

int direction = 0;
int location = 0;

// int map[7][7];
int map[7][7] = {
            {0,0,0,0,0,0,0},
            {1,2,0,2,1,2,0},
            {0,0,0,1,0,1,0},
            {0,2,0,2,0,2,0},
            {0,1,0,0,0,1,0},
            {0,2,1,2,0,2,1},
            {0,0,0,0,0,0,0}
        };
int distances[4];
int visitedCells[16];
int path[16];

double angle;
double pi;


void forward(float distance){
    drive_goto(move(distance), move(distance));
    int loc = updateLocation(&location,direction);
    visitedCells[loc] = 1;
    detectWalls(distances,direction);
    updateMap(distances,location,map);
    prettyPrintMap(map);
}

void getMeasurements(){
    printf("right: %d\n", getRightDist());
    printf("left: %d\n", getLeftDist());
    printf("front: %d\n", ping_cm(8));
    printf("----------------\n");
}

int isCellSurrounded(int visitedCells[], int i){
    int isSur = 1;
    if(i < 12 && !visitedCells[i+4]) isSur = 0;
    if(i % 4 < 3 && !visitedCells[i+1]) isSur = 0;
    if(i < 3 && !visitedCells[i-4]) isSur = 0;
    if(i % 4 && !visitedCells[i-1]) isSur = 0;

    return isSur;
}

int getUncheckedCells(int visitedCells[], int uncheckedCells[]){
    int j = 0;
    for(int i = 0; i < 16; i++){
        if(!visitedCells[i]){
            if(!isCellSurrounded(visitedCells, i)){
                uncheckedCells[j] = i;
                j++;
            }
        }
    }
    return j;
}

int getTargetLocation(int visitedCells[]){
    int locationToGo = -1;
    int minCellsUnchecked = 4;
    int uncheckedCells[4];
    int numCellsUnchecked = getUncheckedCells(visitedCells,uncheckedCells);

    for(int i = 0; i < numCellsUnchecked; i++){
        int tempVisitedCells[16];
        int cellLocation = uncheckedCells[i];
        // copy visited cells array
        for(int j = 0; j < 16; j++){
            tempVisitedCells[j] = visitedCells[j];
        }
        tempVisitedCells[cellLocation] = 1;
        int a[4];
        int numCellsToBeUnchecked = getUncheckedCells(tempVisitedCells,a);
        if(numCellsToBeUnchecked < minCellsUnchecked){
            minCellsUnchecked = numCellsToBeUnchecked;
            locationToGo = cellLocation;
        }
    }

    return locationToGo;
}

void wallFollow(){
    /// enter maze:
    forward(30.5);
    
    // start going through:
    int counter = 0;
    while(counter < 14){

        angle = getAngle();
        
        //if you can turn left: do it
        if (getLeftDist() == 20){
            //turnThroughAngle(90);
            pi = 2.9495;
            while (getAngle() > angle - pi/2){
                drive_speed(-TURN_SPEED,TURN_SPEED);
            }
            updateDirection(&direction,-1);
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
    
        counter++;
    }
}


int main(int argc, const char* argv[])
{
#ifdef BUILDING_IN_SIMULATOR
    simulator_startNewSmokeTrail();
#endif
    prettyPrintMap(map);
    initMap(map);

    for(int i = 0; i < 16; i++){
        visitedCells[i] = 0;
        path[i] = -1;
        if(i < 4) uncheckedCells[i] = 0;
    }
    visitedCells[0] = 1;

    wallFollow();

    getTargetLocation(visitedCells);


    return 0;
}
