#include <stdio.h>
#include <stdlib.h>

const int TURN_STRENGTH = 50;

void movePath(int path[]){
    int i = 0,j;
    int tempDir = 0;

    struct timeval tv;
    gettimeofday(&tv, NULL);
    int lastTime = 0;

    while(path[i+1] >= 0){
        switch(path[i+1]-path[i]){
            case 4:
                j = -tempDir;
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
        int speed = getSpeed(&lastTime,&tv);
        printf("speed: %i\n", speed);
        if(!i && j==1) speed = -18;
        setMove(j*TURN_STRENGTH, speed);
        tempDir += j;
        tempDir %= 4;
        i++;
    }
    setMove(0,getSpeed(&lastTime,&tv));
}