#ifndef ROCKET_STRUCT_HH
#define ROCKET_STRUCT_HH
#include "StateStruct.h"

/* 
     * velocity, height, dragForce - self explanatory
     * simTime - time when data was taken and simulation began
     * timeOfTurnoff - duration of engine working
     * simHeight - height in timestamp n-1 and n respectively
     * rocketMass - mass with motors, without propelant
     * propellantMass - mass of propellant at the time of launch
     * allMass - rocketMass + mass of propellant in time t
     */
struct RocketStruct
{
    float rocketMass;     // rocket mass
    float propellantMass; // propellant mass
    float allMass;
    float timeOfTurnoff;   // t of engine shutdown (all propellant deplated)
    float CdOverMach[301]; // array for Cd values,
                           // CdOverMach[1] contains Cd value for Mach 0.01, CdOverMach[2] for Mach 0.02 and so on

    StateStruct rocketState;

    RocketStruct();

    //RocketStruct(float rm, float pm, float t, StateStruct stat, std::string cdDataName);

    RocketStruct(float rm, float pm, float t, StateStruct stat, float cd[301]);

    //void readCd(std::string cdDataName);

    /// returns simulated apogee:
    float apogeeSimulation(float currentHeight, float lastHeight, float velocity, float time_s);


    float getCd(float machNumber)
    {
        float machTimes100 = machNumber * 100.0;
        if(machTimes100 <= 300.0)
            return CdOverMach[(int)machTimes100]; //conversion from float to int*100
        else return 0;
    }

    float calculateDragForce(float height, float velocity)
    {
        if (velocity > 0)
            return rocketState.calculateAirDensity(height) * pow(velocity, 2) * AREF * getCd(rocketState.calculateMachNumber(height)) * 0.5;
        else return 0;
    }

    void calculateAllMass()
    {
        if (rocketState.simTime < timeOfTurnoff)
            this->allMass = rocketMass + propellantMass * ((timeOfTurnoff - rocketState.simTime) / timeOfTurnoff);
        else
            this->allMass = rocketMass;
    }
};

#endif