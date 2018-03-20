int left, right;
int turnTime = 516;

static void moveFor(int speed, double time)
{
    drive_speed(speed, speed);
    pause(time);
    drive_speed(0, 0);
    pause(1000);
}

// move x small boxes:
static float move(float x){
    return x*100/21;
}

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

// use for when the first move is straight
static void setMove(int amount, int speed){
	int straightTime = 1000;
	int turnTime = 900;
	int speedCap = 6;
	// modded for first turn
	if(speed > speedCap) speed = speedCap;


	if(amount == -2){
		drive_speed(128,128);
		pause(400);
		drive_speed(0,0);
		return;
	}

	drive_speed(128+amount,128-amount);
	if(!amount){
		// pause(straightTime - speed * speed * 0.5);
		pause(straightTime + (speedCap - speed) * 6);
		return;
	}
	pause(turnTime);
}

static void setTurnMove(int amount, int speed, int turnTime){
	int straightTime = 1000;
	int speedCap = 6;
	// modded for first turn
	// if(speed < 3) amount *= 1.4;

	if(speed > speedCap) speed = speedCap;


	drive_speed(128+amount,128-amount);
	if(!amount){
		// pause(straightTime - speed * speed * 0.5);
		pause(straightTime + (speedCap - speed) * 60);
		return;
	}
	pause(turnTime + (speedCap-speed) * -35);
}

int getSpeed(int* lastTime, struct timeval *tv){
	int newLeft,newRight;
	drive_getTicks(&newLeft, &newRight);
	int tickDiffLeft = newLeft - left;
	int tickDiffRight = newRight - right;

	gettimeofday(tv, NULL);
    int time = tv->tv_sec * 1e6 + tv->tv_usec;
    int tempLastTime = *lastTime;

    *lastTime = time;

    tickDiffRight = tickDiffRight > tickDiffLeft ? tickDiffRight : tickDiffLeft;

    return tickDiffRight * 20000 / (time - tempLastTime);
}






