#include "loopTasks.h"

extern Errors errors;
extern Queue queue;
extern DataStruct dataStruct;
extern ImuAPI IMU;

void servoTask(void *arg){
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    uint8_t servoPin = 5;
    Servo servo;
    
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    servo.setPeriodHertz(50);
    servo.attach(servoPin, 500, 2400);

    while(1){
        servo.write(0);
        delay(5000);
        servo.write(10);
        delay(5000);
        servo.write(50);
        delay(5000);
        servo.write(100);
        delay(5000);
    }
}



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
    
    if(!SD_write("/Brake_data.txt", "a.x; a.y; a.z; g.x; g.y; g.z; pressure; altitude; temperature\n"));

    while(1) {
        Serial.println(queue.getNumberOfElements());
        while(queue.getNumberOfElements()){
            String dataFrame = queue.pop() + "\n";

            digitalWrite(onBoardLed, HIGH);  //???
            Serial.println(dataFrame); // Dla debugu
            
            if(!SD_write("/Brake_data.txt", dataFrame)){
                errors.sd_error = SD_WRITE_ERROR;
            }else{
                errors.sd_error = SD_NOERROR;
            }
        
            vTaskDelay(1 / portTICK_PERIOD_MS);
        }
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
        if(errors.imu_error || errors.sd_error){
            digitalWrite(buzzerPin, HIGH); //rozkmina czy lepiej przypsywać stan do zmiennej i tylko przy zmianie stanu używac digitalWrite czy tak jak teraz
            //Serial.println("Error");
        }else{
            digitalWrite(buzzerPin, LOW);
            //Serial.println("No Error");
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

/*
-> zmienienie stanu z AIRBRAKEON na LANDING gdyby nie 

*/
void stateTask(void *arg){
    uint8_t launchDetector = 15;
    uint16_t brakeEjectionAltitude = 1000;
    pinMode(launchDetector, INPUT);

    if(digitalRead(launchDetector) == 0){
        Serial.println("Launch wire, detection error");
    }

    while(1){
        if(dataStruct.rocketState == LAUNCHPAD && digitalRead(launchDetector) == 0){
            dataStruct.rocketState = FLIGHT;
        }else if(dataStruct.rocketState == FLIGHT && (IMU.getAltitude() > brakeEjectionAltitude) /*TIMER insted of altitude or redundant*/){
            dataStruct.rocketState =  AIRBRAKEON;
        }else if(dataStruct.rocketState == AIRBRAKEON /*&& check if rocket is under apoge*/){

        }
    }
}