#ifndef ERROR_STRUCTS_HH
#define ERROR_STRUCTS_HH


enum IMUError{
    IMU_NOERROR = 0,
    IMU_INIT_ERROR,
    IMU_PRESSURE_ERROR,
    IMU_READ_ERROR,
};

enum SDError{
    SD_NOERROR = 0,
    SD_INIT_ERROR,
    SD_WRITE_ERROR,
};

enum RocketError{
    ROCKET_NOERROR = 0,
    ROCKET_INIT_ERROR,
    ROCKET_LIFTOFFDETECTOR_ERROR,
    ROCKET_AIRBRAKE_ERROR,
    ROCKET_IGNITER_ERROR,
    ROCKET_STATE_ERROR,
};

struct Errors{
    IMUError imu_error = IMU_NOERROR;
    SDError sd_error = SD_NOERROR;
    RocketError rocketError = ROCKET_NOERROR;
};



#endif