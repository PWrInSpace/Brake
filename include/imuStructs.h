#ifndef IMUSTRUCTS_HH
#define IMUSTRUCTS_HH
#include <stdint.h>

struct ImuData{
    //Gyroscope
    int16_t gx;
    int16_t gy;
    int16_t gz;

    //Accelerometer
    int16_t ax;
    int16_t az;
    int16_t ay;
    
    int8_t temperature;
    float pressure;
    float altitude;
};

enum AccelerometerScale{
    A_2g = 0,
    A_4g,
    A_8g,
    A_16g,
};

enum GyroscpoeScale{
    G_245dps = 0,
    G_500dps,
    G_1000dps,
    G_2000dps,
};

enum Bandwith{
    B_50Hz = 0,
    B_100Hz,
    B_200Hz,
    B_400Hz,
};

#endif