#include <Arduino.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include "dataStructs.h"
#include "errorStructs.h"
#include "queue.h"
#include "loopTasks.h"
#include "singleTasks.h"
#include "imuAPI.h"
#include "KalmanFilter.h"
#include "flightTimer.h"

Errors errors;
Queue queue;
DataStruct dataStruct;
Servo servo;
FlightTimer flightTimer;
SDCard sdCard(GPIO_NUM_25, GPIO_NUM_27, GPIO_NUM_26, GPIO_NUM_33);

ImuAPI IMU(AccelerometerScale::A_16g, GyroscpoeScale::G_1000dps);
KalmanFilter filter(0.001, 0.003, 0.03);

void setup()
{
  Serial.begin(115200);
  Wire.begin();

  xTaskCreate(errorTask, "error Task", 8192, NULL, 3, NULL);

  if (!dataStruct.rss.init())
  {
    errors.rocketError = ROCKET_INIT_ERROR;
    while (1)
    {
      delay(100);
    }
  }

  if (!sdCard.init())
  {
    errors.sd_error = SD_INIT_ERROR;
    while (1)
    {
      delay(100);
    }
  }

  if (!IMU.begin())
  {
    errors.imu_error = IMU_INIT_ERROR;
    while (1)
    {
      delay(100);
    }
  }

  if (!IMU.setInitPressure())
  {
    errors.imu_error = IMU_PRESSURE_ERROR;
  }

  servoInit();

  dataStruct.rocketState = LAUNCHPAD;

  delay(1000);

  xTaskCreate(stateTask, "state Task", 32768, NULL, 1, NULL);
  
}

void loop()
{
  bool sdWriteStatus = false;
  delay(20);

  IMU.readRawData();
  //dataStruct.imuData = IMU.getRawDataStruct();
  //sdWriteStatus = sdCard.write("/Brake_raw.txt", createDataFrame("RAW"));
  //Serial.println(createDataFrame("RAW")); //debug

  dataStruct.kalmanRoll = filter.update(atan2(dataStruct.imuData.ax * 9.81, dataStruct.imuData.ay * 9.81) * 180 / PI, dataStruct.imuData.gz);
  dataStruct.imuData = IMU.getDataStruct();
  sdWriteStatus &= sdCard.write("/Brake_clc.txt", createDataFrame("CLC"));

  Serial.println(createDataFrame("CLC")); //debug

  sdWriteStatus ? errors.sd_error = SD_NOERROR : errors.sd_error = SD_WRITE_ERROR; //error handling
}