#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "abdrive.h"
#include "simpletext.h"
#include "simpletools.h"                      // simpletools library
#include "ping.h"

#ifdef BUILDING_IN_SIMULATOR
#include "simulator.h"
#endif

int memSize = 88;

int driveL = 75;
int driveR = 75;

void drawA();
void drawE();
void drawK();
void drawL();
void drawI();
void drawH();
void drawF();

int positionStart(){
	drive_goto(-400,-400);
	return 0;
}

int drawWord(char *str){
#ifdef BUILDING_IN_SIMULATOR
	simulator_startNewSmokeTrail();
#endif
  for (int i = 0; i < strlen(str); i++){
  	switch(str[i]){
  		case 'A':
  			drawA();
	  		break;
	  	case 'E':
	  		drawE();
	  		break;
	  	case 'I':
	  		drawI();
	  		break;
	  	case 'H':
	  		drawH();
	  		break;
	  	case 'K':
	  		drawK();
	  		break;
	  	case 'F':
	  		drawF();
	  		break;
	  	case 'L':
	  		drawL();
	  		break;
  	}
#ifdef BUILDING_IN_SIMULATOR
	simulator_stopSmokeTrail();
#endif
  	// space between letters
  	drive_goto(20,20);
#ifdef BUILDING_IN_SIMULATOR
	simulator_startNewSmokeTrail();
#endif
  }

	return 0;
}

int main(int argc, const char* argv[])
{
	positionStart();
	drawWord("AEKIHFL");
  return 0;
  
}

void drawA(){
	drive_goto(-17,18);
    drive_goto(100,100);

    drive_goto(-16,16);
    drive_goto(-100,-100);
    drive_goto(50,50);

    drive_goto(-17,18);
    drive_goto(50,50);
    drive_goto(-50,-50);

    drive_goto(17,-18);
    drive_goto(-50,-50);
    drive_goto(34,-33);
}

void drawE(){
    //making E
 
    //going up:
    drive_goto(-25, 26);
    drive_goto(driveL, driveR);

    // horizontal line:
    drive_goto(25, -26);
    drive_goto(driveL, driveR);
    drive_goto(-driveL, -driveR);

    //going down
    drive_goto(25, -26);
    drive_goto(driveL/2, driveR/2);

    // horizontal line:
    drive_goto(-25,26);
    drive_goto(driveL, driveR);
    drive_goto(-driveL, -driveR);

    //going down
    drive_goto(25, -26);
    drive_goto(driveL/2, driveR/2);

    // horizontal line:
    drive_goto(-25,26);
    drive_goto(driveL, driveR);
    
}

void drawF(){

    //going up:
    drive_goto(-25, 26);
    drive_goto(driveL, driveR);

    // horizontal line:
    drive_goto(25, -26);
    drive_goto(driveL, driveR);
    drive_goto(-driveL, -driveR);

    //going down
    drive_goto(25, -26);
    drive_goto(driveL/2, driveR/2);

    // horizontal line:
    drive_goto(-25,26);
    drive_goto(driveL, driveR);
    drive_goto(-driveL, -driveR);

    //going down
    drive_goto(25, -26);
    drive_goto(driveL/2, driveR/2);
    drive_goto(-25,26);

    drive_goto(driveL/2, driveR/2);
}

void drawH(){

    //going up:
    drive_goto(-25, 26);
    drive_goto(driveL, driveR);

    //going down
    drive_goto(-(driveL/2), -(driveR/2));

    // horizontal line:
    drive_goto(25,-26);
    drive_goto(driveL/2, driveR/2);

     //going up:
     drive_goto(-25, 26);
     drive_goto(driveL/2, driveR/2);

    //going down
    drive_goto(-driveL, -driveR);
    drive_goto(25,-26);
}

void drawI(){;

    //going up:
    drive_goto(-25, 26);
    drive_goto(driveL, driveR);

    //going down:
    drive_goto(-driveL, -driveR);
    drive_goto(25, -26);
}

void drawK(){

    //going up:
    drive_goto(-25, 26);
    drive_goto(driveL, driveR);

    //going down
    drive_goto(-(driveL/2), -(driveR/2));

    // 1st leg line:
    drive_goto(13,-14);
    drive_goto(driveL/2 + 20, driveR/2 + 20);
    drive_goto(-(driveL/2) - 20, -(driveR/2) - 20);

    // 2nd leg line:
    drive_goto(25,-26);
    drive_goto(driveL/2 + 30, driveR/2 + 30);
    drive_goto(-13,14);
}

void drawL(){

    //going up:
    drive_goto(-25, 26);
    drive_goto(driveL, driveR);

    //going down:
    drive_goto(-driveL, -driveR);

    // horizontal line:
    drive_goto(25,-26);
    drive_goto(driveL/2, driveR/2);
}