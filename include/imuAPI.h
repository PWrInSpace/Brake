#ifndef IMUAPI_HH
#define IMUAPI_HH

#include <LPS.h>
#include <LSM6.h>
#include <LIS3MDL.h>
#include "dataStructs.h"
#include "errorStructs.h"

extern Errors errors;

class ImuAPI{
    LSM6 imu;
    LPS ps;
    LIS3MDL mag;
    Imu_data data;
    uint16_t initial_pressure;
    
    public:
    ImuAPI();
    void readData();
    String getData();
};


#endif