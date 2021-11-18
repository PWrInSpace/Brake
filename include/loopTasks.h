#ifndef LOOPTASKS_HH
#define LOOPTASKS_HH

#include "SDcard.h"
#include "dataStructs.h"
#include "errorStructs.h"
#include "singleTasks.h"
#include "queue.h"
#include "imuAPI.h"
#include "SDcard.h"
#include "flightTimer.h"

void imuCalculationTask(void *arg);

void errorTask(void *arg);

void stateTask(void *arg);

void simulationTask(void *arg);

#endif