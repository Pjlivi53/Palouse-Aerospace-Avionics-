#pragma once
#include <MAX6675.h>
#include <SPI.h>

class Ktype_Thermocouple 
{
  private:
    const int DIO;
    const int SCK;
    const int CS;
    MAX6675 sensor;

  public:
    // Initialize the sensor directly in the initializer list
    Ktype_Thermocouple(int dio, int sck, int cs): DIO(dio), SCK(sck), CS(cs), sensor(cs, dio, sck) {}

    void KType_setup() 
    {
      SPI.begin();
      sensor.begin();
      sensor.setSPIspeed(4000000);
    }

    float readKtype() 
    {
      sensor.read();
      return sensor.getCelsius();
    }
};
