#include <stdio.h>
#include <stdlib.h>

#include "abdrive.h"
#include "simpletools.h"

int left, right;
double widthRobot = 105.8; //mm


// clockwise = positive; radians
// 90 degrees = pi/4
static double getAngle(){
	drive_getTicks (&left, &right);
	return (left-right)*3.25/widthRobot;
}

static void turn(int right, int* dir){
	if(right){
		drive_goto(26, -25);
		*dir += 1;
	}else{
		drive_goto(-26, 25);
		*dir -= 1;
	}
}

void updateDirection(int* dir, int amount){
    *dir += amount;
    if(*dir > 3) *dir -= 4;
    if(*dir < 0) *dir += 4;
}