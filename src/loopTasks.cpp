#include "SDcard.h"
#include "loopTasks.h"
#include "dataStructs.h"
#include "errorStructs.h"
#include "queue.h"
#include "imuAPI.h"
#include "SDcard.h"

extern Errors errors;
extern Queue queue;

void IMUTask(void *arg){
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    ImuAPI IMU;

    if(errors.imu_error == IMUError::IMU_INIT_ERROR){
        Serial.println("pętla na imu");
        while(1){};
    }

    while(1){             
        IMU.readRawData();

        queue.push(IMU.getRawData());
        vTaskDelay(10/ portTICK_PERIOD_MS);
    }
}



void SDTask(void *arg) {

    //vTaskDelay(1000 / portTICK_PERIOD_MS);

    SPIClass SPISD(HSPI);
    SPISD.begin(GPIO_NUM_26, GPIO_NUM_27, GPIO_NUM_25);
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    
    if(!SD.begin(SD_CS, SPISD)){
        errors.sd_error = SD_INIT_ERROR;
        while(1){vTaskDelay(1 / portTICK_PERIOD_MS);}
    }

    while(1) {
       Serial.println("Zapisuje");
       
    
        while(queue.getNumberOfElements()){
            
            String dataFrame = queue.pop() + "\n";

            Serial.println(dataFrame); // Dla debugu
            
            if(!SD_write("/Brake_data.txt", dataFrame)){
                errors.sd_error = SD_WRITE_ERROR;
            }else{
                errors.sd_error = SD_NOERROR;
            }
        
            vTaskDelay(1 / portTICK_PERIOD_MS);
        }
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
            digitalWrite(buzzerPin, HIGH);
            //Serial.println("Error");
        }else{
            digitalWrite(buzzerPin, LOW);
            //Serial.println("No Error");
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}