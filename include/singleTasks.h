#ifndef SINGLETASKS_HH
#define SINGLETASKS_HH

#include <Arduino.h>
#include <ESP32Servo.h>
#include "dataStructs.h"

//Servo 
const uint8_t servoClosePosition = 10;
const uint8_t servoOpenPostion = 100;

String createDataFrame(char* pre);

void flightControlTask(void *arg);

void servoInit();

#endif
