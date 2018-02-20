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


const int arrLength = 20 * 2;
const int turnStrength = 10;
const int pointFreq = 1000;
const float speedDec = 1;

int driveSpeed = 30;
int isTurningRight = -1;
int lastTurn = -1;
int numVals = 0;
int lastTime = 0;

int commands[arrLength];

double x, y, theta, target;

// typedef struct list {
//     int val;
//     struct list * next;
// } list_t;

// list_t * commands = NULL;

void setCommand(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  int time = tv.tv_sec * 1e6 + tv.tv_usec;

  commands[numVals] = (time - lastTime);
  printf("x: %d, time: %d\n", isTurningRight, commands[numVals]);
  lastTime = time;
  numVals ++;
  
  lastTurn = isTurningRight;
}

int turn(int right){
  if(right){
    drive_speed(driveSpeed, driveSpeed - turnStrength);
  }else{
    drive_speed(driveSpeed- turnStrength, driveSpeed);
  }
  return right;
}

int main(int argc, const char* argv[])
{
  
  simulator_getPose(&x, &y, &theta);
  target = theta + 3.140;
  drive_speed(-2,2);
  while(theta < target){
    printf("theta: %f\n", theta);
    simulator_getPose(&x, &y, &theta);
    pause(100);
  }
  drive_speed(0,0);
  
  return 0;
}
