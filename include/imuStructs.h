#ifndef IMUSTRUCTS_HH
#define IMUSTRUCTS_HH
#include <stdint.h>

struct ImuData{
    //Gyroscope
    float gx;
    float gy;
    float gz;
    //Accelerometer
    float ax;
    float ay;
    float az;

    //magnetometer
    float mx;
    float my;
    float mz;

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