#ifndef DATA_STRUCTSS_HH
#define DATA_STRUCTSS_HH
#include "imuStructs.h"
#include "errorStructs.h"

//PINS
const uint8_t liftOffDetector = GPIO_NUM_16;
const uint8_t igniterPin = GPIO_NUM_32;

enum State{
    LAUNCHPAD = 0,
    FLIGHT,
    AIRBRAKEON,
    LANDING,
    ONGROUND,
};


struct DataStruct{
    ImuData<int16_t> imuData;
    
    State rocketState = LAUNCHPAD;
    
    uint8_t servoPosition;
    uint8_t igniterState = 0;
    uint8_t airBrakeEjection = 0;
};

#endif