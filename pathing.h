#include <stdio.h>
#include <stdlib.h>

const int TURN_STRENGTH = 57;

void movePath(int path[]){
    int i = 0,j;
    int tempDir = 0;
    int isFirstCellTurn = 0;
    int numStraight = 0;

    struct timeval tv;
    gettimeofday(&tv, NULL);
    int lastTime = 0;

    while(path[i+1] >= 0){
        switch(path[i+1]-path[i]){
            case 4:
                j = - tempDir;
                if(j < -1) j = 1;
                break;
            case 1:
                j = 1 - tempDir;
                break;
            case -4:
                j = 2 - tempDir;
                break;
            case -1:
                j = 3 - tempDir;
                if(j > 1) j = -1;
                break;
        }
        // turn off altered movement if accelerated sufficiently
        if(numStraight == 1) isFirstCellTurn = 0;

        if(!j){
            numStraight ++;
        }else{
            numStraight = 0;
        }

        int speed = getSpeed(&lastTime,&tv);
        if(!i && j==1){
            isFirstCellTurn = 1;
            speed = -1;
        }

        if(isFirstCellTurn){
            setTurnMove(j*TURN_STRENGTH*1.3, speed, 970);
        }else{
            setMove(j*TURN_STRENGTH, speed);
        }

        tempDir += j;
        if(tempDir < 0) tempDir += 4;
        tempDir %= 4;
        i++;
    }
    setMove(0,getSpeed(&lastTime,&tv));
}

