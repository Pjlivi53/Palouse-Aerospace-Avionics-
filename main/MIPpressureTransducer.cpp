#include "MIPpressureTransducer.hpp"

PressureTransducer::PressureTransducer(int pin, float maxPsi, float slopeVal, float interceptVal)
    : analogPin(pin), minVoltage(0.5f), maxVoltage(4.5f), maxPSI(maxPsi), slope(slopeVal), intercept(interceptVal) {}

float PressureTransducer::readPSI() const
{
    int raw = analogRead(analogPin);
    float vout = (raw * maxVoltage) / 1023.0f;
    float psi = slope * (vout - 0.5f) + intercept;
    return psi;
}

float PressureTransducer::readPSIAveraged(int samples) const
{
    if (samples <= 0)
        samples = 1;

    long sum = 0;
    for (int i = 0; i < samples; ++i)
    {
        sum += analogRead(analogPin);
        delayMicroseconds(200);
    }

    float rawAvg = sum / static_cast<float>(samples);
    float vout = (rawAvg * maxVoltage) / 1023.0f;
    float psi = slope * (vout - 0.5f) + intercept;
    return psi;
}
