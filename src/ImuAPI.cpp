#include "imuAPI.h"
#include "errorStructs.h"

extern Errors errors;

ImuAPI::ImuAPI(){
    float averagePressure = 0;
    if (!imu.init() || !ps.init() || !mag.init())
    {
        errors.imu_error = IMU_INIT_ERROR;
        Serial.println("Failed to detect and initialize IMU!"); 
    }

    for(int i=0; i<5; ++i){
        averagePressure += ps.readPressureMillibars();
        vTaskDelay(1/ portTICK_PERIOD_MS);
    }
    initial_pressure = averagePressure/5;

    imu.enableDefault();
    ps.enableDefault();
    mag.enableDefault();
}

void ImuAPI::readData(){
    imu.read();

    data.ax = imu.a.x;
    data.ay = imu.a.y;
    data.az = imu.a.z;

    data.gx = imu.g.x;
    data.gy = imu.g.y;
    data.gz = imu.g.z;

    data.pressure = ps.readPressureMillibars();
    data.temperature = ps.readTemperatureC();
    data.altitude = ps.pressureToAltitudeMeters(data.pressure, initial_pressure);
}

String ImuAPI::getData(){
    char report[100];
    snprintf(report, sizeof(report), "%6d;%6d;%6d;%6d;%6d;%6d;%6f;%f;%6d",
                data.ax, data.ay, data.az,
                data.gx, data.gy, data.gz,
                data.pressure, data.altitude, data.temperature);
    return String(report);
}