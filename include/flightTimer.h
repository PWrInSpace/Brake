#ifndef FLIGHT_TIMER_HH
#define FLIGHT_TIMER_HH

#include <stdint.h>
#include <Arduino.h>

class FlightTimer{
    uint64_t liftOffTime;

    public:
    FlightTimer();
    void startTimer();
    uint64_t getFlightTime();
    uint64_t getLiftOffTime();
};


#endif