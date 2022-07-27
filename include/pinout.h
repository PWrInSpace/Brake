#ifndef PINOUT_H
#define PINOUT_H

#include "driver/gpio.h"

#define IGNITER_1_PIN 0
#define IGNITER_2_PIN 1
#define LIFTOFF_DETECTION_PIN 2
#define BUZZER_PIN 6
#define SERVO_1_PIN 3
#define SERVO_2_PIN 7
#define SDA_PIN 4
#define SCL_PIN 5
#define LED_PIN 8

/**
 * @brief pin initialization
 * 
 */
void IRAM_ATTR liftoff_detection_interrupt(void* arg);
void pin_init();

void buzzer_tick();


#endif