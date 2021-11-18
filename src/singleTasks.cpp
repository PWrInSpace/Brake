#include "singleTasks.h"
#include "dataStructs.h"

extern DataStruct dataStruct;
extern Servo servo;
extern Errors errors;
extern FlightTimer flightTimer;

String createDataFrame(char* pre){
    char dataFrame[200];

    snprintf(dataFrame, sizeof(dataFrame), "%s; %lu; %0.3f; %0.3f; %0.3f; %0.3f; %0.3f; %0.3f; %0.3f; %0.3f; %0.3f; %0.3f; %0.3f; %0.3f; %d; %0.3f; %3d; %d; %d; %d; %d; %d; %d;\n",
            pre, millis() ,dataStruct.imuData.ax, dataStruct.imuData.ay, dataStruct.imuData.az,
            dataStruct.imuData.gx, dataStruct.imuData.gy, dataStruct.imuData.gz,
            dataStruct.imuData.mx, dataStruct.imuData.my, dataStruct.imuData.mz,
            dataStruct.imuData.pressure, dataStruct.imuData.altitude, dataStruct.imuData.temperature, (int)dataStruct.kalmanRoll, 
            dataStruct.simulationApogee, (int) dataStruct.rss.servoPosition, (int) dataStruct.rocketState, (int) dataStruct.rss.airBrakeEjection,
            (int) dataStruct.rss.igniterState, (int) errors.sd_error, (int) errors.imu_error, (int) errors.rocketError);

    return String(dataFrame);
}

void flightControlTask(void *arg){
    const uint64_t breakEjectionTime = 10000;
    const uint64_t deployRecoveryTimeout = 20000;
    const uint64_t breakCloseTime = 30000; 
    bool work = true;
    

    while(work){
        if((breakEjectionTime < flightTimer.getFlightTime()) && (dataStruct.rss.airBrakeEjection == 0)){
            servo.write(servoOpenPostion);
            dataStruct.rss.airBrakeEjection = 1;
            dataStruct.rss.servoPosition = servoOpenPostion;
        }
        
        if(dataStruct.apogeeDetect || (deployRecoveryTimeout < flightTimer.getFlightTime())){
            dataStruct.rss.igniterState = 1;
            digitalWrite(igniterPin, dataStruct.rss.igniterState);
            work = false;
        }

        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    
    //close break at 30s +- 0.1s
    while(flightTimer.getFlightTime() < breakCloseTime){vTaskDelay(100 / portTICK_PERIOD_MS);}  

    servo.write(servoClosePosition); 
    dataStruct.rss.airBrakeEjection = 0;
    dataStruct.rss.servoPosition = servoClosePosition;

    dataStruct.rss.igniterState = 0;
    digitalWrite(igniterPin, dataStruct.rss.igniterState);
    
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