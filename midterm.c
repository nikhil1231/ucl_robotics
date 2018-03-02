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
const float turnAmount = 3.1;

int left, right;
double driveSpeed = 30 * speedUp;
double turnStrength = 7 * speedUp;
int isTurningRight = -1;
int lastTurn = -1;
int numVals = 0;
int lastTime = 0;

int commands[arrLength];
float movingAvgAngles[numAnglesAvg];

double x, y, theta, targetTheta;
double leftDistance, rightDistance; //mm
double widthRobot = 105.8; //mm

void setCommand(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  int time = tv.tv_sec * 1e6 + tv.tv_usec;

  commands[numVals] = (time - lastTime)/1000;
  printf("time: %d\n", commands[numVals]);
  lastTime = time;
  numVals ++;
  
  lastTurn = isTurningRight;
}

int turn(int right){
  if(right){
    drive_speed(driveSpeed, driveSpeed - turnStrength);
  }else{
    drive_speed(driveSpeed - turnStrength, driveSpeed);
  }
  return right;
}

void addMovingAvg(float t){
  for(int i = 1; i < numAnglesAvg; i++){
    movingAvgAngles[i-1] = movingAvgAngles[i];
  }
  movingAvgAngles[numAnglesAvg-1] = t;
}

float getMovingAvg(){
  float avg = 0;
  for(int i = 0; i < numAnglesAvg; i++){
    avg += movingAvgAngles[i];
  }
  return avg / numAnglesAvg;
}

double getAngle(){
  drive_getTicks (&left, &right);
  return (left-right)*3.25/widthRobot;
}

int main(int argc, const char* argv[])
{
  for(int i = 0; i < arrLength; i++){
    commands[i] = -1;
  }
  for(int i = 0; i < numAnglesAvg; i++){
    movingAvgAngles[i] = 0;
  }

#ifdef BUILDING_IN_SIMULATOR
  simulator_startNewSmokeTrail();
#endif

  while(ping_cm(8) > 10){
    int irLeft = getLeftDist(3,0);
    int irRight = getRightDist(3,0);

    isTurningRight = turn(irLeft < irRight);

    if(isTurningRight != lastTurn){
      setCommand();
    }
    theta = getAngle();
    addMovingAvg(theta);
  }
  isTurningRight = !isTurningRight;
  setCommand();

  drive_speed(0,0);
  pause(200);
  while(ping_cm(8) < 10){
    drive_goto(-3,-3);
    pause(100);
  }

  printf("\n---------------\n\n");

  drive_speed(0,0);
  targetTheta = getMovingAvg() + turnAmount;
  pause(2000);
  // drive_goto(-20, -20);
  // pause(200);
  drive_speed(10,-10);
  printf("theta: %f, target theta: %f\n", theta, targetTheta);
  while(theta < targetTheta){
    drive_speed(10,-10);
    theta = getAngle();
    pause(100);
  }
  drive_speed(0,0);
  pause(500);
  printf("theta: %f, target theta: %f\n", theta, targetTheta);

  float mult = 1;

  for(int i = numVals-1; i >= 0; i--){
    int time = commands[i] / mult;

    printf("time: %d\n", time);

    driveSpeed *= mult;
    turnStrength *= mult;

    turn(isTurningRight);
    isTurningRight = !isTurningRight;
    pause(time);
  }

  drive_speed(200,200);
  pause(2000);
  
  return 0;
}
