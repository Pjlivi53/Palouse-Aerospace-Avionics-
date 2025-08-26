#include "KType.hpp"
#include "LoadCell.hpp"
#include "MIPpressureTransducer.hpp"

// Initiate all 3 sensors
Ktype_Thermocouple thermocouple(26, 27, 28);     // DIO, SCK, CS
LoadCell loadCell(22, 23, 696.0);                // DOUT, SCK, Cal factor
MIPAnalogSensor pressureSensor(A0, 100.0);       // Analog pin, Max PSI

void setup() 
{
  Serial.begin(115200);

  // Initialize each sensor
  thermocouple.KType_setup();
  loadCell.setup();
  pressureSensor.setup();

  Serial.println("All sensors initialized.");
}

void loop()
{
  float tempC = thermocouple.readKtype();   // Temperature in Celsius
  float load  = loadCell.read();            // Load cell reading
  float psi   = pressureSensor.read();      // Pressure in PSI

  // Output all data in consistent format
  Serial.print("TEMP (°C): ");
  Serial.print(tempC);
  Serial.print(" | LOAD: ");
  Serial.print(load);
  Serial.print(" | PRESSURE (psi): ");
  Serial.println(psi);
  if ((tempC >= 100) || (load >= 100) || (psi >= 1000))
  {
    /*
    block of code for writing to ssd
    */
  }
  delay(500);  // Adjust based on logging or refresh rate
}