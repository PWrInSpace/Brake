#ifndef LOOPTASKS_HH
#define LOOPTASKS_HH

#include "SDcard.h"
#include "dataStructs.h"
#include "errorStructs.h"
#include "queue.h"
#include "imuAPI.h"
#include "SDcard.h"
#include <ESP32Servo.h>
#include <Wire.h>

void servoTask(void *arg);

void SDTask(void *arg);

void errorTask(void *arg);

void stateTask(void *arg);

#endif