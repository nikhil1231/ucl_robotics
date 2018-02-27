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

const int arrLength = 200;
const float speedUp = 2;
const int numAnglesAvg = 5;
const float turn90 = 3.1;

int left, right;
double driveSpeed = 30 * speedUp;
double theta, targetTheta;
double widthRobot = 105.8; //mm

int commands[arrLength];

double getAngle(){
  drive_getTicks (&left, &right);
  return (left-right)*3.25/widthRobot;
}

void turn(int right){
  if(right){
    drive_goto(-26, 25);
  }else{
    drive_goto(26, -25);
  }

  // targetTheta = getAngle() + right * turn90;
  // while(theta < targetTheta){
  //   drive_speed(10,-10);
  //   theta = getAngle();
  //   pause(100);
  // }
}

int main(int argc, const char* argv[])
{
#ifdef BUILDING_IN_SIMULATOR
  simulator_startNewSmokeTrail();
#endif

  for(int i = 0; i < arrLength; i++){
    commands[i] = -1;
  }

  while(ping_cm(8) > 10){
    int irLeft = getLeftDist(1,0);
    int irRight = getRightDist(1,0);
  }
  
  return 0;
}
