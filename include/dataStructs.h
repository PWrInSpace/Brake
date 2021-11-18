#ifndef DATA_STRUCTSS_HH
#define DATA_STRUCTSS_HH
#include "imuStructs.h"
#include "errorStructs.h"

extern Errors errors;

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

struct RocketStateSensors{
    uint8_t servoPosition;
    uint8_t igniterState = 0;
    uint8_t airBrakeEjection = 0;
    bool successfulInit = false;

    bool init(){
        if(digitalRead(liftOffDetector) == 0){
            Serial.println("Launch wire, detection error");   
            errors.rocketError = ROCKET_LIFTOFFDETECTOR_ERROR;
    
        }else if(airBrakeEjection){
            Serial.println("air brake ejection error");
            errors.rocketError = ROCKET_AIRBRAKE_ERROR;

        }else if(igniterState){
            Serial.println("Wrong igniter state");
            errors.rocketError = ROCKET_IGNITER_ERROR;        
    
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
    uint8_t kalmanRoll = 0;
};

#endif