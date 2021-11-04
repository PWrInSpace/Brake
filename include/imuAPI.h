#ifndef IMUAPI_HH
#define IMUAPI_HH

#include <LPS.h>
#include <LSM6.h>
#include <LIS3MDL.h>
#include "imuStructs.h"
#include "errorStructs.h"

extern Errors errors;

constexpr uint8_t AccReg[4] = {(0<<2), (1<<3), (1<<3) | (1<<2), (1<<2)};
constexpr float AccFactor[4] = {0.061, 0.122, 0.244, 0.488};
constexpr uint8_t GyroReg[4] = {};
constexpr uint8_t bandwithReg[4] = {(1<<1) | (1<<0), (1<<1), (1<<0), (0<<0)};


class ImuAPI{
    LSM6 imu;
    LPS ps;
    LIS3MDL mag;

    ImuData data;
    float initPressure;
    Bandwith bandwith;
    AccelerometerScale accScale;
    GyroscpoeScale gyrScale;

    public:
    ImuAPI(GyroscpoeScale _gyrScale = GyroscpoeScale::G_245dps, 
            AccelerometerScale _accScale = AccelerometerScale::A_2g,
            Bandwith _bandwith = Bandwith::B_400Hz);
    bool begin();
    void setInitPressure();
    void readRawData();
    String getRawData();
    ImuData getRawDataStruct();
    float getAltitude();
    
    private:
    void LSM6SetReg();
};


#endif