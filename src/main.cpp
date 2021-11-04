#include <Arduino.h>

#include "dataStructs.h"
#include "errorStructs.h"
#include "queue.h"
#include "loopTasks.h"
#include "singleTasks.h"
#include "imuAPI.h"

Errors errors;
Queue queue;
DataStruct dataStruct;


char report[80];

ImuAPI IMU;

void setup()
{
  Serial.begin(115200);
  Wire.begin();

  xTaskCreate(servoTask, "servo Task",   65536, NULL, 1, NULL);
  xTaskCreate(SDTask,    "SD Task",      65536, NULL, 2, NULL);
  xTaskCreate(errorTask, "error Task",   16384, NULL, 3, NULL);
  
  if(!IMU.begin()){
    errors.imu_error = IMU_INIT_ERROR;
    while(1){delay(100);}
  }
  IMU.setInitPressure();
  dataStruct.rocketState = LAUNCHPAD;
  //delay(1000);
}

void loop()
{  
  delay(50);
  IMU.readRawData();
  dataStruct.imuData = IMU.getRawDataStruct();

  String data = createDataFrame(); 
  Serial.println(data);
  
  queue.push(createDataFrame());
 
}