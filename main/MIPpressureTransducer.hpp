#pragma once
#include <Arduino.h>

class PressureTransducer
{
private:
    const int analogPin;
    const float minVoltage;
    const float maxVoltage;
    const float maxPSI;
    const float slope;
    const float intercept;

public:
    PressureTransducer(int pin, float maxPsi, float slope, float intercept);
    float readPSI() const;
    float readPSIAveraged(int samples) const;
};

