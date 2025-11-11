#pragma once
#include <HX711_ADC.h>
#include <EEPROM.h>

class LoadCell
{
private:
    const int Dout;
    const int SCK;
    const float CalFactor;
    HX711_ADC sensor;

public:
    LoadCell(int dout, int sck, float cal);
    void setup();
    float getCalFactor() const;
    float read();
};

