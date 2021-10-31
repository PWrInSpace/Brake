#ifndef ERROR_STRUCTS_HH
#define ERROR_STRUCTS_HH


enum IMUError{
    IMU_NOERROR = 0,
    IMU_INIT_ERROR,
    IMU_READ_ERROR,
};

enum SDError{
    SD_NOERROR = 0,
    SD_INIT_ERROR,
    SD_OPEN_ERROR,
    SD_WRITE_ERROR,
};

struct Errors{
    IMUError imu_error;
    SDError sd_error;
};


#endif