#include "singleTasks.h"
#include "dataStructs.h"

extern DataStruct dataStruct;
extern Servo servo;
extern Errors errors;

String createDataFrame(char* pre){
    char dataFrame[200];

    snprintf(dataFrame, sizeof(dataFrame), "%s; %6f; %6f; %6f; %6f; %6f; %6f; %6f; %6f; %2d; %3d; %d; %d; %d; %d; %d; %d;",
            pre, dataStruct.imuData.ax, dataStruct.imuData.ay, dataStruct.imuData.az,
            dataStruct.imuData.gx, dataStruct.imuData.gy, dataStruct.imuData.gz,
            dataStruct.imuData.pressure, dataStruct.imuData.altitude, dataStruct.imuData.temperature,
            (int) dataStruct.servoPosition, (int) dataStruct.rocketState, (int) dataStruct.airBrakeEjection,
            (int) dataStruct.igniterState, (int) errors.sd_error, (int) errors.imu_error, (int) errors.rocketError);

    return String(dataFrame);
}

void flightControlTask(void *arg){
    const uint32_t timer = millis();
    const uint32_t breakEjectionTime = 1000;
    const uint32_t deployRecoveryTime = 2000;
    bool work = true;
    

    while(work){
        if((breakEjectionTime < millis() - timer) && (dataStruct.airBrakeEjection == 0)){
            servo.write(servoOpenPostion);
            dataStruct.airBrakeEjection = 1;
            dataStruct.servoPosition = servoOpenPostion;
        }
        
        if((deployRecoveryTime < millis() - timer)){
            dataStruct.igniterState = 1;
            digitalWrite(igniterPin, dataStruct.igniterState);
            work = false;
        }

        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    servo.write(servoClosePosition); //close servo after parachute deploy
    dataStruct.airBrakeEjection = 0;
    dataStruct.servoPosition = servoClosePosition;

    dataStruct.igniterState = 0;
    digitalWrite(igniterPin, dataStruct.igniterState);
    
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