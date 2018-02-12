#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "abdrive.h"
#include "simpletext.h"
#include "simpletools.h"
#include "ping.h"

int getIr(int samples, int delay, int dac, int freq, int inputPin){
	int ir = 0;
	int samplesArr[samples];
	for(int i = 0; i < samples; i++){
		int temp = 0;
		for(int dacVal = 0; dacVal < 160; dacVal += 8)  
		{                                               
			dac_ctr(dac, 0, dacVal);                       
			freqout(freq, 1, 38000);
			temp += input(inputPin);
		}
		samplesArr[i] = temp;
		// pause(delay);
   	}
   	for(int i = 0; i < samples; i++){
   		ir += samplesArr[i];
   	}
    return ir/samples;
}

int getLeftDist(int samples, int delay){
    return getIr(samples, delay, 26, 11, 10);
}
      
int getRightDist(int samples, int delay){
    return getIr(samples, delay, 27, 1, 2);
}

