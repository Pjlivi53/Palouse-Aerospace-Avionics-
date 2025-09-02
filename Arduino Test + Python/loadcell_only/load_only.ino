#include "LoadCell.hpp"

// Initiate Load Cell
LoadCell loadCell(22, 23, 8661.0);                // DOUT, SCK, Cal factor

void setup() 
{
  Serial.begin(115200);

  loadCell.setup();

  Serial.println("Load cell initialized.");
}

void loop()
{
  float load  = loadCell.read();            // Load cell reading

  // Output all data in consistent format
  Serial.print(" | LOAD: ");
  Serial.print(load);
  Serial.print("\n\r");
  delay(500);  // Adjust based on logging or refresh rate
}
