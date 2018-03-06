#include <stdio.h>
#include <stdlib.h>

#include "simpletools.h"

const int IR_LIMIT = 20;
const int US_LIMIT = 30;

void detectWalls(int dists[], int direction){
    // Treat distances like compass, not relative to the robot.
    int* leftDist = &dists[(direction)%4];
    int* frontDist = &dists[(direction+1)%4];
    int* rightDist = &dists[(direction+2)%4];
    dists[(direction+3)%4] = -1;

    *leftDist = getLeftDist();
    *frontDist = ping_cm(8);
    *rightDist = getRightDist();

    // see if there's a wall around
    *leftDist = *leftDist < IR_LIMIT ? 2 : 0;
    *rightDist = *rightDist < IR_LIMIT ? 2 : 0;
    *frontDist = *frontDist < US_LIMIT ? 2 : 0;
}

// returns true if all 4 surrounding cells are visited.
int isCellSurrounded(int visitedCells[], int i){
    int isSur = 1;
    if(i < 12 && !visitedCells[i+4]) isSur = 0;
    if(i % 4 < 3 && !visitedCells[i+1]) isSur = 0;
    if(i < 3 && !visitedCells[i-4]) isSur = 0;
    if(i % 4 && !visitedCells[i-1]) isSur = 0;

    return isSur;
}

// returns non-surrounded cells.
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

// get optimal cell to go to if there are unchecked cells
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

void initMap(int map[7][7]){
    for(int i = 0; i < 7; i++){
        for(int j = 0; j < 7; j++){
            map[i][j] = i%2==1 && j%2==1 ? 3 : 0;
        }
    }
}

void printMap(int map[7][7]){
    for(int i = 6; i >= 0; i--){
        for(int j = 0; j < 7; j++){
            int c = map[i][j];
            printf("%i ",c);
        }
        printf("\n");
    }
    printf("\n");
}

void prettyPrintMap(int map[7][7]){
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

int updateLocation(int* location, int direction){
    switch(direction){
        case 0:
            if(*location < 12) *location += 4;
            break;
        case 1:
            if(*location % 4 < 3) *location += 1;
            break;
        case 2:
            if(*location > 3) *location -= 4;
            break;
        case 3:
            if(*location % 4) *location -= 1;
            break;
    }
    return *location;
}

void updateMap(int dists[], int location, int map[7][7]){
    // north
    if(location < 12 && dists[1] > 0) map[(location/4)*2+1][(location%4)*2] = dists[1];
    // east
    if(location % 4 < 3 && dists[2] > 0) map[(location/4)*2][(location%4)*2+1] = dists[2];
    // south
    if(location > 3 && dists[3] > 0) map[(location/4)*2-1][(location%4)*2] = dists[3];
    // west
    if(location % 4 && dists[0] > 0) map[(location/4)*2][(location%4)*2-1] = dists[0];
}


