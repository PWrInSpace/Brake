#include "singleTasks.h"
#include "dataStructs.h"

extern DataStruct dataStruct;

String createDataFrame(){
    char dataFrame[150];

    snprintf(dataFrame, sizeof(dataFrame), "%6d; %6d; %6d; %6d; %6d; %6d; %6f; %6f; %2d; %d; %d; %d; %3d;",
            dataStruct.imuData.ax, dataStruct.imuData.az, dataStruct.imuData.ay,
            dataStruct.imuData.gx, dataStruct.imuData.gz, dataStruct.imuData.gy,
            dataStruct.imuData.pressure, dataStruct.imuData.altitude, dataStruct.imuData.temperature,
            (int) dataStruct.sdStatus, (int) dataStruct.imuStatus, (int) dataStruct.rocketState, (int) dataStruct.servoPosition);

    return String(dataFrame);
}