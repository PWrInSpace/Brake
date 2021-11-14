#ifndef IMUSTRUCTS_HH
#define IMUSTRUCTS_HH
#include <stdint.h>

template <typename T>
struct ImuData{
    //Gyroscope
    T gx;
    T gy;
    T gz;
    //Accelerometer
    T ax;
    T ay;
    T az;
    
    int8_t temperature;
    float pressure;
    float altitude;

    explicit operator ImuData<float>(){
        ImuData<float> x;
        x.gx = gx;
        x.gy = gy;
        x.gz = gz;

        x.ax = ax;
        x.ay = ay;
        x.az = az;

        x.temperature = temperature;
        x.pressure = pressure;
        x.altitude = altitude;

        return x;
    }
};

template struct ImuData<int16_t>;
template struct ImuData<float>;


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