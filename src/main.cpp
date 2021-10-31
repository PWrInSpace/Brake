#include <Arduino.h>
#include <Wire.h>
#include <ESP32Servo.h>
#include "dataStructs.h"
#include "errorStructs.h"
#include "queue.h"
#include "loopTasks.h"
Errors errors;
Queue queue;
Servo servo;
uint8_t servoPin = 27;

char report[200];

void setup()
{
  Serial.begin(115200);
  Wire.begin();

  xTaskCreate(IMUTask, "IMU Task", 65536, NULL, 2, NULL);
  xTaskCreate(SDTask,   "SD Task", 65536, NULL, 2, NULL);
  /*
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  servo.setPeriodHertz(50);
  servo.attach(servoPin, 500, 2400);
  */
}

void loop()
{
  delay(100);
}