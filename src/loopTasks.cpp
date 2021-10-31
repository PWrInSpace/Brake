#include "SDcard.h"
#include "loopTasks.h"
#include "dataStructs.h"
#include "errorStructs.h"
#include "queue.h"
#include "imuAPI.h"

extern Errors errors;
extern Queue queue;

void IMUTask(void *arg){
    ImuAPI IMU;

    while(1){             
        IMU.readData();

        queue.push(IMU.getData());
        vTaskDelay(100/ portTICK_PERIOD_MS);
    }
}



void SDTask(void *arg) {

    vTaskDelay(1000 / portTICK_PERIOD_MS);

   // SPIClass SPISD(HSPI);
   // SPISD.begin(GPIO_NUM_25, GPIO_NUM_26, GPIO_NUM_15);
   // SPI.setClockDivider(SPI_CLOCK_DIV2);
    
   // if(!SD.begin(SD_CS, SPISD)){
   //     errors.sd_error = SD_INIT_ERROR;
        //while(1){vTaskDelay(1 / portTICK_PERIOD_MS);}
   // }

    while(1) {
       
        while(queue.getNumberOfElements()){
            
            String dataFrame = queue.pop();

            Serial.println(dataFrame); // Dla debugu
            
            //SD_write("/Brake_data.txt", dataFrame);
        
            vTaskDelay(1 / portTICK_PERIOD_MS);
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}