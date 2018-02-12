#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

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

const int arrLength = 20 * 2;
const int turnStrength = 10;
const int pointFreq = 1000;
const float speedDec = 0.5;

int driveSpeed = 30;
int isTurningRight = -1;
int lastTurn = -1;
int numVals = 0;
int lastTime = 0;

int commands[arrLength];
// int yPos[arrLength];

double x, y, theta;

// int isNear(int x1, int y1, int x2, int y2, int range){
//   if(abs(x1 - x2) < range && abs(y1 - y2) < range){
//     return 1;
//   }
//   return 0;
// }

// double getAngle(int x1, int x2, int )

void setCommand(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  int time = tv.tv_sec * 1e6 + tv.tv_usec;

  
  commands[numVals*2] = isTurningRight;
  commands[numVals*2 + 1] = time - lastTime;
  printf("x: %d, time: %d\n", commands[numVals*2], commands[numVals*2 +1]);
  lastTime = time;
  numVals ++;
  
  
  lastTurn = isTurningRight;
}


int main(int argc, const char* argv[])
{

  // init array
  for(int i = 0; i < arrLength; i++){
    commands[i] = -1;
  }

#ifdef BUILDING_IN_SIMULATOR
  simulator_startNewSmokeTrail();
#endif
  drive_speed(driveSpeed, driveSpeed);

  while(ping_cm(8) > 8)
  {
    
    int irLeft = getLeftDist(3,0);
    int irRight = getRightDist(3,0);

    if(irLeft < irRight){
      drive_speed(driveSpeed, driveSpeed - turnStrength);
      isTurningRight = 1;
    }else{
      drive_speed(driveSpeed- turnStrength, driveSpeed);
      isTurningRight = 0;
    }

    if(isTurningRight != lastTurn){
      setCommand();
    }
    
    // printf("IR left: %d, IR right %d\n", irLeft, irRight);
  }
  setCommand();
  printf("\n");
  printf("---------------\n");

  // for(int i = 0; i < arrLength; i+=2){
  //   printf("x: %d, time: %d\n", commands[i], commands[i+1]);
  // }
  printf("\n");

  drive_speed(0,0);
  drive_goto(-20, -20);
  drive_goto(54, -52);

  driveSpeed *= speedDec;

  for(int i = numVals-1; i >= 0; i--){
    int turn = commands[i*2];
    int time = commands[i*2+1] / 1000;

    printf("x: %d, time: %d\n", turn, time);
    if(turn){
      drive_speed(driveSpeed - turnStrength, driveSpeed);
    }else{
      drive_speed(driveSpeed, driveSpeed - turnStrength);
    }
    pause(time);
  }
  
  return 0;
}
