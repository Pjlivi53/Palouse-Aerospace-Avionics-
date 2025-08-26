#pragma once
#include <Arduino.h>

class MIPAnalogSensor 
{
  private:
    const int analogPin;
    const float minVoltage = 0.5;   // Voltage at 0 psi
    const float maxVoltage = 4.5;   // Voltage at full scale
    const float maxPSI;             // Full scale pressure (e.g., 100.0)

  public:
  
    MIPAnalogSensor(int pin, float maxPressure) : analogPin(pin), maxPSI(maxPressure) {}

    void setup() {
      pinMode(analogPin, INPUT);
      Serial.println("MIP analog sensor ready.");
    }

    float read(int samples = 5) 
    {
        int raw = analogRead(analogPin);
        float voltage = raw * (5.0 / 1023.0);
        float psi = (voltage - minVoltage) * (maxPSI / (maxVoltage - minVoltage));
        return constrain(psi, 0, maxPSI);
    }
};
