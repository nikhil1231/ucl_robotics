#include <stdio.h>
#include <stdlib.h>

#include "abdrive.h"
#include "simpletools.h"

int left, right;
double widthRobot = 105.8; //mm


static void turnThroughAngle(double angle)
{
    int ticks = (int) round(angle*0.284);
    drive_speed(-ticks, ticks);
}

static double getAngle(){
	drive_getTicks (&left, &right);
	return (left-right)*3.25/widthRobot;
}

static void turn(int right){
	float turn90 = 3.1;
	if(right == 1){
		drive_goto(26, -25);
	}else{
		drive_goto(-26, 25);
	}

  // targetTheta = getAngle() + right * turn90;
  // while(theta < targetTheta){
  //   drive_speed(10,-10);
  //   theta = getAngle();
  //   pause(100);
  // }
}