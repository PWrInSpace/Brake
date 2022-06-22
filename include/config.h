#ifndef CONFIG_HH
#define CONFIG_HH

#include <stdint.h>

#define BRAKE_EJECION_TIME 3000 //czas otworznia brejka
#define RECOV_TIME_DEPLOY 15000 //czas odpalenia gdy imu nie wykryje 
#define IMU_ALLOW_TIME 8500 //od ktorej sekundy lotu imu moze odpalic zapalnik
#define BRAKE_CLOSE_TIME 40000 //czas zamkniecia prejka
#define APOGEE_CONFIRMATION_TIME 500 //czas od wykycia apogeum do jego potwierdzenia

const uint8_t liftOffDetector = 32;
const uint8_t igniterPin = 5;
const uint8_t buzzerPin = 12;

#endif 