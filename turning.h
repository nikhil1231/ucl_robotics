#include <stdio.h>
#include <stdlib.h>

#include "abdrive.h"
#include "simpletools.h"

static void turnThroughAngle(double angle)
{
    int ticks = (int) round(angle*0.284);
    drive_speed(-ticks, ticks);
}

