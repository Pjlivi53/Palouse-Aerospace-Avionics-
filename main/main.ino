#include "MIPpressureTransducer.hpp"
#include "LoadCell.hpp"
#include "REYAX_RYL896.hpp"

inline void sendLogRow(float load, float chamberPsi, float runTankPsi) 
{
  Serial2.print(millis());  Serial2.print(',');
  Serial2.print(load, 3);   Serial2.print(',');
  Serial2.print(chamberPsi, 3); Serial2.print(',');
  Serial2.println(runTankPsi, 3);   
}


static inline String dataToString(float load, float chamberPsi, float runTankPsi) 
{
  String message;
  message = String(millis()) + "," + String(load, 2) + "," + String(chamberPsi, 2) + "," + String(runTankPsi, 2);
  return message;
}

LoadCell loadCell(4, 5, 3925.74f);//Dout,SCK,CalFactor
PressureTransducer CombustionChamberSensor(A0, 500.0, 120.4, 5.5097);//Analog pin, maxpsi, slope of cal data, intercept of cal data PSI = Slope * (Vo - minVoltage) + Intercept
PressureTransducer  RunTankSensor(A1, 1500.0, 363.22, 12.268);

float avgZeroLoad, avgZeroChamber, avgZeroRunTank;
float zeroSumLoad = 0, zeroSumChamber = 0, zeroSumTank = 0;

int ZeroCounts = 50;//How many times take data at ambient conditions

void setup()
{
  Serial1.begin(115200);//bound rate
  Serial2.begin(57600);//bound rate

  loadCell.setup();
  CombustionChamberSensor.setup();
  RunTankSensor.setup();

  for (int i = 0; i < ZeroCounts; i++)
  {
    zeroSumLoad += loadCell.read();
    zeroSumChamber += CombustionChamberSensor.read();
    zeroSumTank += RunTankSensor.read();
    delay(100);//ms
  }
  avgZeroLoad = zeroSumLoad / (float)ZeroCounts;
  avgZeroChamber = zeroSumChamber / (float)ZeroCounts;
  avgZeroRunTank = zeroSumTank / (float)ZeroCounts;

  String LoRaMessage = dataToString(avgZeroLoad, avgZeroChamber, avgZeroRunTank);

  LoRaSend(LoRaMessage);

  sendLogRow(avgZeroLoad, avgZeroChamber, avgZeroRunTank);


  LoRaSend("All Sensors Ready");

}

void loop()
{
  float load = loadCell.read();
  float chamPSI = CombustionChamberSensor.read();
  float runPSI = RunTankSensor.read();

  sendLogRow(load,chamPSI,runPSI);

  String message = dataToString(load, chamPSI, runPSI);


  LoRaSend(message);

  delay(50);//ms
}
