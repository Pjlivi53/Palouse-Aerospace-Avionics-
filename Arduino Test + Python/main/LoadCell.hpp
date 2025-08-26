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
    LoadCell(int dout, int sck, float cal): Dout(dout), SCK(sck), CalFactor(cal), sensor(dout, sck) {}

    void setup()
     {
      sensor.begin();                // just call begin with no args
      sensor.start(2000, true);      // 2s to stabilize
      sensor.setCalFactor(CalFactor);
    }

    float read() 
    {
      if (sensor.update()) 
      {      
        return sensor.getData();
      }
      return 0.0;
    }
};
