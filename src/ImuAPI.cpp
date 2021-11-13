#include "imuAPI.h"
#include "errorStructs.h"

extern Errors errors;

ImuAPI::ImuAPI(AccelerometerScale _accScale, GyroscpoeScale _gyrScale, Bandwith _bandwith){
    imu = LSM6();
    ps = LPS();
    mag = LIS3MDL();
    gyroScale = _gyrScale;
    accScale = _accScale;
    bandwith = _bandwith;
}

bool ImuAPI::begin(){
    if (!imu.init()){
        Serial.println("Failed to detect and initialize IMU :C");
        return false;

    }else if(!ps.init()){
        Serial.println("Filed to detect and initialize PS :C");
        return false;
    
    }else if(!mag.init()){
        Serial.println("Filed to detect and initialize MAG :C");
        return false;
    
    }
    
    //imu.enableDefault();
    this->LSM6SetReg();
    ps.enableDefault();
    mag.enableDefault();
    return true;
}

//TO DO: change too low pass filter
bool ImuAPI::setInitPressure(){
    float press = 0;

    for(int i = 0; i<5; ++i){
        press += ps.readPressureMillibars();
        delay(10);
    }
    initPressure = press/5.0;

    if(ps.readPressureMillibars() - abs(initPressure) > 5){
        initPressure = 1023; //average sea level pressure
        return false;
    }    

    return true;
}

void ImuAPI::readRawData(){
    imu.read();

    data.ax = imu.a.x;
    data.ay = imu.a.y;
    data.az = imu.a.z;

    data.gx = imu.g.x;
    data.gy = imu.g.y;
    data.gz = imu.g.z;

    data.pressure = ps.readPressureMillibars();
    data.temperature = ps.readTemperatureC();
    data.altitude = ps.pressureToAltitudeMeters(data.pressure, initPressure);
}

String ImuAPI::createDataReport(ImuData reportData){
    char report[100];
    snprintf(report, sizeof(report), "%6d;%6d;%6d;%6d;%6d;%6d;%6f;%6f;%2d",
            reportData.ax, reportData.ay, reportData.az,
            reportData.gx, reportData.gy, reportData.gz,
            reportData.pressure, reportData.altitude, reportData.temperature);
    return String(report);
}

ImuData ImuAPI::createCountedData(){
    ImuData countedData = this->data;
    countedData.ax *= accFactor[accScale];
    countedData.ay *= accFactor[accScale];
    countedData.az *= accFactor[accScale];

    countedData.gx *= gyroFactor[gyroScale];
    countedData.gy *= gyroFactor[gyroScale];
    countedData.gz *= gyroFactor[gyroScale];
    
    return countedData;
} 

String ImuAPI::getRawData(){
    return createDataReport(this->data);
}

String ImuAPI::getData(){
    return createDataReport(this->createCountedData());
}

ImuData ImuAPI::getRawDataStruct(){
    return data;
}

ImuData ImuAPI::getDataStruct(){
    return this->createCountedData();
}

float ImuAPI::getAltitude(){
    return data.altitude;
}

void ImuAPI::LSM6SetReg(){
    uint8_t reg = 0b00000000;
    
    //accelerometr
    reg |= accReg[accScale];
    reg |= bandwithReg[bandwith];
    reg |= 1 << 7; //ODR high performance 1.66kHz TO DO: odr choice
    
    imu.writeReg(imu.regAddr::CTRL1_XL, reg);
    
    //gyroscope 
    reg = 0b00000000;
    reg |= gyroReg[gyroScale];
    reg |= 1<<7; //ODR high performance 1.66kHz

    imu.writeReg(imu.regAddr::CTRL2_G, reg);
    //Comon
    reg = 0b00000100; //IF_IN = 1 automatically increment register address
    imu.writeReg(imu.regAddr::CTRL3_C, reg);
}

/*
ImuAPI::ImuAPI(GyroscpoeScale _gyrScale, AccelerometerScale _accScale, Bandwith _bandwith){
    gyrScale = _gyrScale;
    accScale = _accScale;
    bandwith = _bandwith;

    //float averagePressure = 0;
    if (!imu.init())
    {
        errors.imu_error = IMU_INIT_ERROR;
        Serial.println("Failed to detect and initialize IMU!"); 
    }
    
    imu.enableDefault();

    ps.enableDefault();
    mag.enableDefault();
    }

}




bool ImuAPI::begin(){
    if(errors.imu_error != 0){
        return false; //constructor error :C
    }
    //TO DO:
    //... reg set for all sensors
    return true;
}


 

    imu.enableDefault();
    ps.enableDefault();
    mag.enableDefault();




void ImuAPI::readRawData(){
    imu.read();

    data.ax = imu.a.x;
    data.ay = imu.a.y;
    data.az = imu.a.z;

    data.gx = imu.g.x;
    data.gy = imu.g.y;
    data.gz = imu.g.z;

    //data.pressure = ps.readPressureMillibars();
    //data.temperature = ps.readTemperatureC();
    //data.altitude = ps.pressureToAltitudeMeters(data.pressure, initial_pressure);
}

String ImuAPI::getRawData(){
    char report[100];
    snprintf(report, sizeof(report), "%6d;%6d;%6d;%6d;%6d;%6d",
                data.ax, data.ay, data.az,
                data.gx, data.gy, data.gz);
    return String(report);
}
*/