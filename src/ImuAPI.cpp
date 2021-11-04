#include "imuAPI.h"
#include "errorStructs.h"

extern Errors errors;

ImuAPI::ImuAPI(GyroscpoeScale _gyrScale, AccelerometerScale _accScale, Bandwith _bandwith){
    imu = LSM6();
    ps = LPS();
    mag = LIS3MDL();
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
    
    imu.enableDefault();
    ps.enableDefault();
    mag.enableDefault();
    return true;
}

void ImuAPI::setInitPressure(){
    float press = 0;

    for(int i = 0; i<5; ++i){
        press += ps.readPressureMillibars();
        delay(10);
    }
    initPressure = press/5.0;
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
String ImuAPI::getRawData(){
    char report[100];
    snprintf(report, sizeof(report), "%6d;%6d;%6d;%6d;%6d;%6d;%6f;%6f;%2d",
            data.ax, data.ay, data.az,
            data.gx, data.gy, data.gz,
            data.pressure, data.altitude, data.temperature);
    return String(report);
}

ImuData ImuAPI::getRawDataStruct(){
    return data;
}

float ImuAPI::getAltitude(){
    return data.altitude;
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

void ImuAPI::LSM6SetReg(){
    uint8_t reg = 0b00000000;
    
    //accelerometr
    reg |= AccReg[accScale];
    reg |= bandwithReg[bandwith];
    //reg ODR
    
    imu.writeReg(imu.regAddr::CTRL1_XL, reg);
    
    //gyroscope 
    reg = 0;


    //Comon

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