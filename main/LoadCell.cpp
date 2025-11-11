#include "LoadCell.hpp"
#include <Arduino.h>

LoadCell::LoadCell(int dout, int sck, float cal)
    : Dout(dout), SCK(sck), CalFactor(cal), sensor(dout, sck) {}

void LoadCell::setup()
{
    sensor.begin();
    unsigned long stabilizingTime = 2000;
    sensor.start(stabilizingTime);
    sensor.setCalFactor(CalFactor);
}

float LoadCell::getCalFactor() const
{
    return CalFactor;
}

float LoadCell::read()
{
    if (sensor.update())
        return sensor.getData();
    return 0.0f;
}
