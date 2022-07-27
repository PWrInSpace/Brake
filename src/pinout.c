#include "../include/pinout.h"
#include "esp_log.h"

#define PIN_TAG "PINOUT ERROR"

/**
 * @brief interrupt for liftoff detection
 * 
 * @param arg NULL
 */
void IRAM_ATTR liftoff_detection_interrupt(void *arg){
  printf("XDD\n");
}

void pin_init(){
  gpio_reset_pin(IGNITER_1_PIN); //reset pin
  gpio_set_direction(IGNITER_1_PIN, GPIO_MODE_OUTPUT); //set pin as output
  gpio_set_level(IGNITER_1_PIN, 0); //set to low
  //ESP_LOGD(PIN_TAG, "GPIO PIN INIT %d", IGNITER_1);

  gpio_reset_pin(IGNITER_2_PIN);
  gpio_set_direction(IGNITER_2_PIN, GPIO_MODE_OUTPUT);
  gpio_set_level(IGNITER_2_PIN, 0);
  
  gpio_reset_pin(BUZZER_PIN);
  gpio_set_direction(BUZZER_PIN, GPIO_MODE_OUTPUT);
  gpio_set_level(BUZZER_PIN, 0);
  
  gpio_reset_pin(LIFTOFF_DETECTION_PIN);
  gpio_set_direction(LIFTOFF_DETECTION_PIN, GPIO_MODE_INPUT); //set liftoff detection pin as input
  //gpio_set_pull_mode(LIFTOFF_DETECTION_PIN, GPIO_PULLUP_ONLY);
  gpio_set_intr_type(LIFTOFF_DETECTION_PIN, GPIO_INTR_POSEDGE); //set interrupt of rising edge
  gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1); //instal gpio isr service
  gpio_isr_handler_add(LIFTOFF_DETECTION_PIN, liftoff_detection_interrupt, NULL); //hook isr handler
}
