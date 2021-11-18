#include"../include/StateStruct.h"

StateStruct::StateStruct(float pos[2], float diti, float startTime)
{
    this->simHeight[0] = pos[0];
    this->simHeight[1] = pos[1];
    this->dt = diti;
    this->simTime = startTime;
    this->velocity = (simHeight[1] - simHeight[0]) / dt;
}