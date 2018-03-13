#include <stdio.h>
#include <stdlib.h>

#include "abdrive.h"
#include "simpletools.h"

int left, right;
int turnTime = 516;
double widthRobot = 105.8; //mm


static void moveFor(int speed, double time)
{
    drive_speed(speed, speed);
    pause(time);
    drive_speed(0, 0);
}

// move x small boxes:
static float move(float x){
    return x*100/21;
}

// clockwise = positive; radians
// 90 degrees = pi/4
static double getAngle(){
	drive_getTicks (&left, &right);
	return (left-right)*3.25/widthRobot;
}

// static void updateDirection(int* dir, int amount){
//     *dir += amount;
//     if(*dir > 3) *dir -= 4;
//     if(*dir < 0) *dir += 4;
// }

static void turn(int amount, int* dir){
	*dir += amount;
    if(*dir > 3) *dir -= 4;
    if(*dir < 0) *dir += 4;

	int x = amount > 0 ? 50 : -50;
	drive_speed(x,-x);
    pause(abs(516*amount));
    drive_speed(0,0);
    pause(1000);
}
