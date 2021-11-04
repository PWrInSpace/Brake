#ifndef DATA_STRUCTS_HH
#define DATA_STRUCTS_HH
#include "imuStructs.h"
#include "errorStructs.h"

enum State{
    LAUNCHPAD = 0,
    FLIGHT,
    AIRBRAKEON,
    LANDING,
    ONGROUND,
};

struct DataStruct{
    ImuData imuData;

    SDError sdStatus;
    IMUError imuStatus;
    
    State rocketState = LAUNCHPAD;
    
    uint8_t servoPosition;
};

#endif