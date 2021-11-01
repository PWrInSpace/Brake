#ifndef DATASTRUCTS_HH
#define DATASTRUCTS_HH
#include <stdint.h>

struct Imu_data{
    //Gyroscope
    uint16_t gx;
    uint16_t gy;
    uint16_t gz;

    //Accelerometer
    uint16_t ax;
    uint16_t az;
    uint16_t ay;
    
    uint8_t temperature;
    float pressure;
    float altitude;
};

#endif