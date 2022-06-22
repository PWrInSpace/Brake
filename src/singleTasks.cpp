#include "singleTasks.h"
#include "dataStructs.h"
#include "config.h"

extern DataStruct dataStruct;
extern Servo servo;
extern Errors errors;
extern FlightTimer flightTimer;

String createDataFrame(char* pre){
    char dataFrame[200];

    snprintf(dataFrame, sizeof(dataFrame), "%s; %lu; %0.3f; %0.3f; %0.3f; %0.3f; %0.3f; %0.3f; %0.3f; %0.3f; %0.3f; %0.3f; %0.3f;  %d; %0.3f; %0.3f; %3d; %d; %d; %d; %d; %d; %d; %d;\n",
            pre, millis() ,dataStruct.imuData.ax, dataStruct.imuData.ay, dataStruct.imuData.az,
            dataStruct.imuData.gx, dataStruct.imuData.gy, dataStruct.imuData.gz,
            dataStruct.imuData.mx, dataStruct.imuData.my, dataStruct.imuData.mz,
            dataStruct.imuData.pressure, dataStruct.imuData.altitude, dataStruct.imuData.temperature, 
            dataStruct.kalmanRoll, dataStruct.simulationApogee, (int) dataStruct.rss.servoPosition, 
            (int) dataStruct.rocketState,  (int) dataStruct.rss.airBrakeEjection, (int) dataStruct.rss.igniterState,
            (int) dataStruct.apogeeDetect, (int) errors.sd_error, (int) errors.imu_error, (int) errors.rocketError);

    return String(dataFrame);
}

void flightControlTask(void *arg){
    const uint64_t brakeEjectionTime = BRAKE_EJECION_TIME;
    const uint64_t deployRecoveryTimeout = RECOV_TIME_DEPLOY;
    const uint64_t brakeCloseTime = BRAKE_CLOSE_TIME; 
    bool work = true;
    

    while(work){
        if((brakeEjectionTime < flightTimer.getFlightTime()) && (dataStruct.rss.airBrakeEjection == 0)){
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
    
    //close brake at 30s +- 0.1s
    while(flightTimer.getFlightTime() < brakeCloseTime){vTaskDelay(100 / portTICK_PERIOD_MS);}  

    servo.write(servoClosePosition); 
    dataStruct.rss.airBrakeEjection = 0;
    dataStruct.rss.servoPosition = servoClosePosition;

    dataStruct.rss.igniterState = 0;
    digitalWrite(igniterPin, dataStruct.rss.igniterState);
    
    vTaskDelete(NULL);  //Close task
}

void servoInit(){
    uint8_t servoPin = GPIO_NUM_18;
    //uint8_t servoClosePosition = 10;

    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    servo.setPeriodHertz(50);
    servo.attach(servoPin, 500, 2400);

    servo.write(servoClosePosition);
}