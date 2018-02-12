#include "abdrive.h"

#ifdef BUILDING_IN_SIMULATOR
#include "simulator.h"
#endif

int main(int argc, const char* argv[])
{
    // Enable the smoke trail
    #ifdef BUILDING_IN_SIMULATOR
    simulator_startNewSmokeTrail();
    #endif

    int driveL = 308;
    int driveR = 308;

    // Drive it
    drive_goto(driveL, driveR);
    drive_goto(51, 0);

    drive_goto(driveL, driveR);
    drive_goto(51, 0);

    drive_goto(driveL, driveR);
    drive_goto(51, 0);

    drive_goto(driveL, driveR);
    drive_goto(51, 0);
}