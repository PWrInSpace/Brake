#ifndef IMUAPI_HH
#define IMUAPI_HH

#include <LPS.h>
#include <LSM6.h>
#include <LIS3MDL.h>
#include "dataStructs.h"
#include "errorStructs.h"

extern Errors errors;

constexpr uint8_t AccReg[4] = {(0<<2), (1<<3), (1<<3) | (1<<2), (1<<2)};
constexpr float AccFactor[4] = {0.061, 0.122, 0.244, 0.488};
constexpr uint8_t BandWithReg[4] = {(0<<0), (1<<0), (1<<1), (1<<1) | (1<<0) };
enum AccelerometerScale{
    A_2g = 0,
    A_4g,
    A_8g,
    A_16g,
};

enum GyroscpoeScale{
    G_245dps = 0,
    G_500dps = (1<<0),
    G_1000dps = (1<<1),
    G_2000dps = (1<<1) + (1<<0),
};

enum BandWith{
    B_50Hz = 0x11,
    B_100Hz = 0x10,
    B_200Hz = 0x01,
    B_400Hz = 0x00,
};

class ImuAPI{
    LSM6 imu;
    LPS ps;
    LIS3MDL mag;
    Imu_data data;
    float initial_pressure;
    BandWith bandWith;
    AccelerometerScale accScale;
    GyroscpoeScale gyrScale;

    public:
    ImuAPI(GyroscpoeScale _gyrScale = GyroscpoeScale::G_245dps, 
            AccelerometerScale _accScale = AccelerometerScale::A_2g,
            BandWith _bandWith = BandWith::B_400Hz);
    bool begin();

    
    void readRawData();
    String getRawData();
    private:
    void LSM6SetReg();
};


#endif