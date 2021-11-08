#include "singleTasks.h"
#include "dataStructs.h"

extern DataStruct dataStruct;
extern Servo servo;

String createDataFrame(){
    char dataFrame[150];

    snprintf(dataFrame, sizeof(dataFrame), "%6d; %6d; %6d; %6d; %6d; %6d; %6f; %6f; %2d; %d; %d; %d; %3d;",
            dataStruct.imuData.ax, dataStruct.imuData.az, dataStruct.imuData.ay,
            dataStruct.imuData.gx, dataStruct.imuData.gz, dataStruct.imuData.gy,
            dataStruct.imuData.pressure, dataStruct.imuData.altitude, dataStruct.imuData.temperature,
            (int) dataStruct.sdStatus, (int) dataStruct.imuStatus, (int) dataStruct.rocketState, (int) dataStruct.servoPosition);

    return String(dataFrame);
}

void flightControlTask(void *arg){
    const uint32_t timer = millis();
    const uint32_t breakEjectionTime = 1000;
    const uint32_t deployRecoveryTime = 2000;
    bool work = true;
    

    while(work){
        if((breakEjectionTime < millis() - timer) && (dataStruct.airBrakeEjection == 0)){
            dataStruct.airBrakeEjection = 1;
            servo.write(servoOpenPostion);
        }
        
        if((deployRecoveryTime < millis() - timer)){
            digitalWrite(igniterPin, HIGH);
            dataStruct.igniterState = 1;
            work = false;
        }

        vTaskDelay(1 / portTICK_PERIOD_MS);
    }

    servo.write(servoClosePosition); //close servo after parachute deploy
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    digitalWrite(igniterPin, LOW);
    vTaskDelete(NULL);  //Close task
}

void servoInit(){
    uint8_t servoPin = 5;
    uint8_t servoClosePosition = 0;

    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    servo.setPeriodHertz(50);
    servo.attach(servoPin, 500, 2400);

    servo.write(servoClosePosition);
}