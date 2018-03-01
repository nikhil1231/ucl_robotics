#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "abdrive.h"
#include "simpletext.h"
#include "simpletools.h"
#include "ping.h"

int getIr(int dac, int freq, int inputPin){
	int temp = 0;
	for(int dacVal = 0; dacVal < 160; dacVal += 8)  
	{                                               
		dac_ctr(dac, 0, dacVal);                       
		freqout(freq, 1, 38000);
		temp += input(inputPin);
	}
    return temp;
}

int getLeftDist(){
    return getIr(26, 11, 10);
}
      
int getRightDist(){
    return getIr(27, 1, 2);
}

