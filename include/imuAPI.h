#ifndef IMUAPI_HH
#define IMUAPI_HH

#include <LPS.h>
#include <LSM6.h>
#include <LIS3MDL.h>
#include "imuStructs.h"
#include "errorStructs.h"

extern Errors errors;

const uint8_t accReg[4] = {(0<<2), (1<<3), (1<<3) | (1<<2), (1<<2)};
const float accFactor[4] = {0.061, 0.122, 0.244, 0.488};
const uint8_t gyroReg[4] = {(0<<3), (1<<3), (1<<4), (1<<4) | (1<<3)};
const float gyroFactor[4] = {8.75, 17.50, 35.0, 70.0};
const uint8_t bandwithReg[4] = {(1<<1) | (1<<0), (1<<1), (1<<0), (0<<0)};

class ImuAPI{
    LSM6 imu;
    LPS ps;
    LIS3MDL mag;

    ImuData<int16_t> data;
    float initPressure;
    Bandwith bandwith;
    AccelerometerScale accScale;
    GyroscpoeScale gyroScale;

    public:
    ImuAPI( AccelerometerScale _accScale = AccelerometerScale::A_2g,
            GyroscpoeScale _gyrScale = GyroscpoeScale::G_245dps,
            Bandwith _bandwith = Bandwith::B_400Hz);
    bool begin();
    bool setInitPressure();
    void readRawData();
    String getRawData();
    String getData();
    ImuData<int16_t> getRawDataStruct();
    ImuData<float> getDataStruct();
    float getAltitude();
    
    private:
    ImuData<float> createCountedData();
    void LSM6SetReg();
    template<typename T>
    String createDataReport(ImuData<T> reportData);
};

String createDataReport(ImuData<int16_t> reportData);
String createDataReport(ImuData<float> reportData);
#endif