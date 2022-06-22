#include "flightTimer.h"

FlightTimer::FlightTimer(){
    liftOffTime = 0;
}

void FlightTimer::startTimer(){
    liftOffTime = millis();
}

uint64_t FlightTimer::getFlightTime(){
    return millis() - liftOffTime;
}

uint64_t FlightTimer::getLiftOffTime(){
    return liftOffTime;
}