#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

int turnStrength = 3;
int driveSpeed = 30;
int endDistance = 7;
int targetDistFromWall = 1;
int distTol = 0;
int hasTurned = 0;
int hasHitend = 0;
int isReversing = 0;

// void makePara(int samples, int delay){
//   float deltaD = 0;
//   int samplesArr[samples];
//   for(int i = 0; i < samples; i++){
//     samplesArr[i] = getLeftDist(3,1);
//     if(i > 0) samplesArr[i-1] -= samplesArr[i];
//     moveFor(driveSpeed, delay);
//   }
//   for(int i = 0; i < samples - 1; i++){
//     deltaD += samplesArr[i];
//   }
//   deltaD /= samples;

//   turnThroughAngle(atan(deltaD*200/delay) * -180/3.14159);

//   printf("%f",deltaD);
// }


int main(int argc, const char* argv[])
{

#ifdef BUILDING_IN_SIMULATOR
  simulator_startNewSmokeTrail();
#endif
  drive_speed(driveSpeed, driveSpeed);

  int pingDist = ping_cm(8);
  
  while(pingDist > 5)
  {
    
    // makePara(3, 200);
    int irLeft = getLeftDist(1,1);

    int distFromWall = targetDistFromWall - irLeft;

    pingDist = ping_cm(8);
    
    if(pingDist > 10){
      if(distFromWall > distTol){
        drive_speed(driveSpeed + turnStrength * distFromWall, - turnStrength * distFromWall);
      }else if(distFromWall < - distTol){
        drive_speed(driveSpeed + turnStrength * distFromWall, - turnStrength * distFromWall);
      }else{
          drive_speed(driveSpeed,driveSpeed);
      }
    }else{
      if(distFromWall > distTol){
        drive_speed(20 + turnStrength * distFromWall, - turnStrength * distFromWall);
      }else if(distFromWall < - distTol){
        drive_speed(turnStrength * distFromWall, 20 + turnStrength * distFromWall);
      }else{
          drive_speed(driveSpeed,driveSpeed);
      }
      // if(!hasHitend)
      //   drive_speed(-driveSpeed, -driveSpeed);
      // hasHitend ++;
    }
    
    
    printf("IR dist: %d\n", irLeft);
    printf("Front dist: %d\n", pingDist);
    pause(10);
  }
  
  drive_speed(0, 0);
  turnThroughAngle(-20);
  pause(500);
  
  return 0;
}
