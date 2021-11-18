#include "loopTasks.h"

extern Errors errors;
extern Queue queue;
extern DataStruct dataStruct;
extern ImuAPI IMU;
extern FlightTimer flightTimer;
extern SDCard sdCard;

void imuCalcuationsTask(void *arg){
    float maxAltitude = 0;
    float currentAltitude = 0;
    uint64_t apogeeTimer = 0;
    const uint64_t apogeeConfirmTime = 500; //ms
    const uint64_t igniterSafeTime = 15000;
    const uint64_t timeout = 30000; //ms
    bool apogeeAltitudeConfirm = false;
    bool apogeeAccZConfirm = false;

    while(1){
        currentAltitude = dataStruct.imuData.altitude;

        if(maxAltitude < currentAltitude){
            maxAltitude = currentAltitude;
            apogeeTimer = milli();
        }

        if(dataStruct.imuData.az < 0.70){
            appogeAccZConfirm = true;
        }

        if(apogeeTimer - millis() > apogeeConfirmTime){
            apogeeAltitudeConfirm = true;
        }

        if((apogeeAltitudeConfirm && apogeeAccZConfirm) && flightTimer.getFlightTime > igniterSafeTime){
            dataStruct.apogeeDetect = true;
            sdCard.write("/Break_logs.txt", String(Sprintf("Apogee detected : %f; %f; %d; %d;", maxAltitude, flightTimer.getFlightTime, apogeeAltitudeConfirm, apogeeAccZConfirm)));
            vTaskDelete(NULL);
        }else if(flightTimer.getFlightTime > timeout){
            sdCard.write("/Break_logs.txt", String(Sprintf("Apogee not detected : %f; %f; %d; %d;", maxAltitude, flightTimer.getFlightTime, apogeeAltitudeConfirm, apogeeAccZConfirm)));
            vTaskDelete(NULL);
        }

        vTaskDelay(40/portTICK_PERIOD_MS);
    }
}


//Simple error handling
void errorTask(void *arg){ 
    uint8_t buzzerPin = 4;
    errors.imu_error = IMU_NOERROR;
    errors.sd_error = SD_NOERROR;
    
    pinMode(buzzerPin, OUTPUT);
    
    while(1){
        if(errors.imu_error || errors.sd_error || errors.rocketError){
            digitalWrite(buzzerPin, HIGH); 
        }else{
            digitalWrite(buzzerPin, LOW);
        }

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}


/**
 * Czy wyłączyć airBrake jeżeli igniter się nie odpali?
 * bo 30s
 *  
 */
void stateTask(void *arg){
    if(!dataStruct.rss.successfulInit){
        errors.rocketError = ROCKET_INIT_ERROR;
        while(1) {vTaskDelay(100 / portTICK_PERIOD_MS);}
    }

    if(dataStruct.rocketState != LAUNCHPAD){
        errors.rocketError = ROCKET_STATE_ERROR;
        while(1) {vTaskDelay(100 / portTICK_PERIOD_MS);}
    }


    while(1){
        if((dataStruct.rocketState == LAUNCHPAD) && ((digitalRead(liftOffDetector) == 0) || dataStruct.imuData.altitude > 100)){
            flightTimer.startTimer();
            dataStruct.rocketState = FLIGHT;
            xTaskCreate(flightControlTask,  "flight control task",    16384,  NULL, 1, NULL); 
            xTaskCreate(imuCalcuationsTask, "imuC calculations task", 32768,  NULL, 1, NULL);
            xTaskCreate(simulationTask,     "simulation Task",        65 536, NULL, 2, NULL);
            Serial.println("FLIGHT");
        
        }else if(dataStruct.rocketState == FLIGHT && dataStruct.rss.airBrakeEjection != 0){
            dataStruct.rocketState = AIRBRAKEON;
            Serial.println("AIRBRAKE");
 
        }else if(dataStruct.rocketState == AIRBRAKEON && dataStruct.rss.igniterState != 0){
            dataStruct.rocketState = LANDING;
            Serial.println("LAnDING");
 
        }else if(dataStruct.rocketState == LANDING && dataStruct.imuData.altitude < 100.0){
            dataStruct.rocketState = ONGROUND;
            Serial.println("On ground");
 
        }
        vTaskDelay(2 / portTICK_PERIOD_MS);
    }
}

void simulationTask(void *arg){

    while(1){
        //dataStruct.simulationApogee =  zapis wyliczonego apeogeum do głównej struktury
        vTaskDelay(1/ portTICK_PERIOD_MS);
    }
}