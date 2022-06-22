#ifndef DATA_STRUCTSS_HH
#define DATA_STRUCTSS_HH
#include "imuStructs.h"
#include "errorStructs.h"
#include "config.h"

extern Errors errors;

enum State{
    LAUNCHPAD = 0,
    FLIGHT,
    AIRBRAKEON,
    LANDING,
    ONGROUND,
};

struct RocketStateSensors{
    uint8_t servoPosition;
    uint8_t igniterState = 0;
    uint8_t airBrakeEjection = 0;
    bool successfulInit = false;

    bool init(){
        pinMode(liftOffDetector, INPUT);
        if(digitalRead(liftOffDetector) == 0){
            Serial.println("Launch wire, detection error");   
            errors.rocketError = ROCKET_LIFTOFFDETECTOR_ERROR;
        }

        errors.rocketError == ROCKET_NOERROR ? successfulInit = true : successfulInit = false;

        return successfulInit;
    }
};

struct DataStruct{
    ImuData imuData;
    
    State rocketState = LAUNCHPAD;

    RocketStateSensors rss;

    double simulationApogee = 0; 
    double kalmanRoll = 0;

    bool apogeeDetect = false;
};

#endif