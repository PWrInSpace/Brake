/*
The sensor outputs provided by the library are the raw
16-bit values obtained by concatenating the 8-bit high and
low accelerometer and gyro data registers. They can be
converted to units of g and dps (degrees per second) using
the conversion factors specified in the datasheet for your
particular device and full scale setting (gain).

Example: An LSM6DS33 gives an accelerometer Z axis reading
of 16276 with its default full scale setting of +/- 2 g. The
LA_So specification in the LSM6DS33 datasheet (page 11)
states a conversion factor of 0.061 mg/LSB (least
significant bit) at this FS setting, so the raw reading of
16276 corresponds to 16276 * 0.061 = 992.8 mg = 0.9928 g.
*/

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