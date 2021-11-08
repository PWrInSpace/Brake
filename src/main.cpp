#include <Arduino.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include "dataStructs.h"
#include "errorStructs.h"
#include "queue.h"
#include "loopTasks.h"
#include "singleTasks.h"
#include "imuAPI.h"

Errors errors;
Queue queue;
DataStruct dataStruct;
Servo servo;
bool isSaving;

char report[80];

ImuAPI IMU;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  
  isSaving = false;

  xTaskCreate(stateTask, "state Task",    65536, NULL, 1, NULL);
  xTaskCreate(SDTask,    "SD Task",      65536, NULL, 2, NULL);
  xTaskCreate(errorTask, "error Task",   16384, NULL, 3, NULL);
  
  servoInit();

  if(!IMU.begin()){
    errors.imu_error = IMU_INIT_ERROR;
    while(1){delay(100);}
  }
  IMU.setInitPressure();  //Sprawdzać czy nie dało jakiejś wartości dziwnej +- 5 hpa od tego co przyszło
  dataStruct.rocketState = LAUNCHPAD;
  //delay(1000);
}

void loop()
{  
  delay(50);
  if(!isSaving){

    IMU.readRawData();
    dataStruct.imuData = IMU.getRawDataStruct();

    String data = createDataFrame(); 
    Serial.println(data + String("    main"));
    
    queue.push(createDataFrame());
  }
 
}