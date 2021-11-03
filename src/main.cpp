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
uint8_t servoPin = 5;

char report[200];

void setup()
{
  Serial.begin(115200);
  Wire.begin();//


  xTaskCreate(IMUTask,   "IMU Task",   65536, NULL, 2, NULL);
  xTaskCreate(SDTask,    "SD Task",    65536, NULL, 2, NULL);
  xTaskCreate(errorTask, "error Task", 16384, NULL, 3, NULL);
  
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  servo.setPeriodHertz(50);
  servo.attach(servoPin, 500, 2400);
  
  
}

void loop()
{
  servo.write(100);
  delay(1000);
  servo.write(0);
  delay(1000);
  //delay(5000);
  //esp_sleep_enable_timer_wakeup(5* 10e5);
  //esp_deep_sleep_start();
  
}