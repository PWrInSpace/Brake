#include "loopTasks.h"

extern Errors errors;
extern Queue queue;
extern DataStruct dataStruct;
extern ImuAPI IMU;
extern bool isSaving;


void SDTask(void *arg) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    uint8_t onBoardLed = 2;
    pinMode(onBoardLed, OUTPUT);
    digitalWrite(onBoardLed, LOW);

    SPIClass SPISD(HSPI);
    SPISD.begin(GPIO_NUM_26, GPIO_NUM_27, GPIO_NUM_25);
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    
    if(!SD.begin(SD_CS, SPISD)){
        errors.sd_error = SD_INIT_ERROR;
        while(1){vTaskDelay(1 / portTICK_PERIOD_MS);}
    }

    //raw data
    SD_write("/Brake_raw_data.txt", "a.x; a.y; a.z; g.x; g.y; g.z; pressure; altitude; temperature; servo position; \\
                    Rocket state, Air brake state, IgniterState, SD error, Imu error, Rocket error\n");
    
    //calcualted data
    SD_write("/Brake_data.txt", "a.x; a.y; a.z; g.x; g.y; g.z; pressure; altitude; temperature; servo position; \\
                    Rocket state, Air brake state, IgniterState, SD error, Imu error, Rocket error\n");
    while(1) {
        //Serial.println(queue.getNumberOfElements()); //debug
        while(queue.getNumberOfElements()){
            String path;
            isSaving = true;
            String dataFrame = queue.pop() + "\n";

            digitalWrite(onBoardLed, HIGH);
            
            switch(dataFrame[0]){
                case 'R':
                    path = "/Brake_raw_data.txt";
                    break;
                case 'C':
                    path = "/Break_data.txt";
                    break;
                default:
                    errors.sd_error = SD_WRITE_ERROR;
            }

            if(!SD_write(path, dataFrame)){
                errors.sd_error = SD_WRITE_ERROR;
            }else{
                errors.sd_error = SD_NOERROR;
            }
    
            //Serial.println(dataFrame + String("    sd")); // Dla debugu
        
            vTaskDelay(1 / portTICK_PERIOD_MS);
        }
        isSaving = false;
        digitalWrite(onBoardLed, LOW);

        vTaskDelay(2000 / portTICK_PERIOD_MS);
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
            digitalWrite(buzzerPin, HIGH); //rozkmina czy lepiej przypsywać stan do zmiennej i tylko przy zmianie stanu używac digitalWrite czy tak jak teraz
            //while() blokowanie porgramu na state LAUNCHPAD podczas erroru
            
        }else{
            digitalWrite(buzzerPin, LOW);
            //Serial.println("No Error");
        }

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}


/**
 * Czy wyłączyć airBrake jeżeli igniter się nie odpali?
 * 
 *  
 */
void stateTask(void *arg){
    pinMode(liftOffDetector, INPUT);
    pinMode(igniterPin, OUTPUT);
    digitalWrite(igniterPin, LOW);

    //debug
    if(digitalRead(liftOffDetector) == 0){
        Serial.println("Jest 0");
    }else{
        Serial.println("Jest 1");
    }

    //chyba coś jeszcze 
    if(digitalRead(liftOffDetector) == 0){
        Serial.println("Launch wire, detection error");   
        errors.rocketError = ROCKET_LIFTOFFDETECTOR_ERROR;
    
    }else if(dataStruct.airBrakeEjection){
        Serial.println("air brake ejection error");
        errors.rocketError = ROCKET_AIRBRAKE_ERROR;

    }else if(dataStruct.igniterState){
        Serial.println("Wrong igniter state");
        errors.rocketError = ROCKET_IGNITER_ERROR;        
    
    }else if(dataStruct.rocketState != LAUNCHPAD){
        Serial.println("Invalid rocket state");
        errors.rocketError = ROCKET_STATE_ERROR;
    }

    if(errors.rocketError != ROCKET_NOERROR){
        while(1) {vTaskDelay(100 / portTICK_PERIOD_MS);}
    }

    while(1){
        if((dataStruct.rocketState == LAUNCHPAD) && (digitalRead(liftOffDetector) == 0)){
            dataStruct.rocketState = FLIGHT;
            Serial.println("FLIGHT");
            xTaskCreate(flightControlTask, "flight control task", 16384, NULL, 1, NULL); 

        }else if(dataStruct.rocketState == FLIGHT && dataStruct.airBrakeEjection != 0){
            dataStruct.rocketState = AIRBRAKEON;
            Serial.println("AIRBRAKE");
 
        }else if(dataStruct.rocketState == AIRBRAKEON && dataStruct.igniterState != 0){
            dataStruct.rocketState = LANDING;
            Serial.println("LAnDING");
 
        }else if(dataStruct.rocketState == LANDING && dataStruct.imuData.altitude < 100.0){
            dataStruct.rocketState = ONGROUND;
            Serial.println("On ground");
 
        }
        vTaskDelay(2 / portTICK_PERIOD_MS);
    }
}