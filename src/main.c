#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

//user files inclue
#include "../include/pinout.h"
#include "../include/state_machine.h"

void app_main() {
  state_machine_t state_machine;
  pin_init(); //init i/o pins
  state_machine_init(&state_machine);


  while(1){
    switch(state_machine.state){
      case LAUNCHPAD:
        break;
      case FLIGHT:
        break;
      case AIRBRAKE_EJECTION:
        break;
      case LANDING:
        break;
      case ONGROUND:
        break;
    }

    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}