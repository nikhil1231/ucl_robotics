#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "abdrive.h"
#include "simpletext.h"
#include "simpletools.h"                      // simpletools library
#include "ping.h"

int main(int argc, const char* argv[])
{
  // Spin through 360 degrees
  while(1)
    drive_speed(102, -102);

  return 0;
  
}
