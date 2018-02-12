#include <stdio.h>
#include <stdlib.h>

#include "abdrive.h"
#include "simpletools.h"

static void moveFor(int speed, double time)
{
    drive_speed(speed, speed);
    pause(time);
    drive_speed(0, 0);
}

