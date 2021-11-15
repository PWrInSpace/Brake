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

Errors errors;
Queue queue;
DataStruct dataStruct;
Servo servo;
bool isSaving;

char report[80];

ImuAPI IMU(AccelerometerScale::A_16g, GyroscpoeScale::G_1000dps);
KalmanFilter filter(0.001, 0.003, 0.03);

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  
  isSaving = false;

  xTaskCreate(stateTask,      "state Task",      65536, NULL, 1, NULL);
  xTaskCreate(SDTask,         "SD Task",         65536, NULL, 2, NULL);
  xTaskCreate(errorTask,      "error Task",      16384, NULL, 3, NULL);
  //xTaskCreate(simulationTask, "simulation task", 65536, NULL, 4, NULL);
  
  servoInit();

  if(!IMU.begin()){
    errors.imu_error = IMU_INIT_ERROR;
    while(1){delay(100);}
  }
  
  if(!IMU.setInitPressure()){
    errors.imu_error = IMU_PRESSURE_ERROR;
  }

  dataStruct.rocketState = LAUNCHPAD;
  //delay(1000);
}

void loop()
{  
  delay(50);
  if(!isSaving){

    IMU.readRawData();
    dataStruct.imuData = IMU.getRawDataStruct();
    Serial.println(createDataFrame("RAW")); //debug
    dataStruct.kalmanRoll = filter.update(atan2(dataStruct.imuData.ax * 9.81, dataStruct.imuData.ay * 9.81) * 180 / PI, dataStruct.imuData.gz);
    queue.push(createDataFrame("RAW"));
    dataStruct.imuData = IMU.getDataStruct();
    Serial.println(createDataFrame("CLC")); //debug
    queue.push(createDataFrame("CLC"));
  }
 
}