#pragma once
#include <Arduino.h>

class PressureTransducer 
{
  private:
    const int analogPin;
    const float minVoltage = 0.5;   // Voltage at 0 psi
    const float maxVoltage = 4.5;   // Voltage at full scale
    const float maxPSI;
    const float slope;
    const float intercept;

  public:
  
    PressureTransducer(int pin, float maxPressure, float m, float b) : analogPin(pin), maxPSI(maxPressure), slope(m), intercept(b) {}

    void setup() 
    {
      pinMode(analogPin, INPUT);
      Serial.println("Pressure Transducer is Ready");
    }

    float read(int samples = 5) 
    {
        long sum = 0;
        for (int i = 0; i < samples; i++) {
            sum += analogRead(analogPin);
            delayMicroseconds(200); 
        }
        float rawAvg = sum / (float)samples;
        // Convert ADC to voltage
        float vout = (rawAvg * maxVoltage) / 1023.0f;  
        float psi = slope * (vout - 0.5f) + intercept;
        return psi;
    }
};

