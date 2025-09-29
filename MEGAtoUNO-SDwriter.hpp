#pragma once
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>

class SD_UNO
{
  private:
    static const uint8_t RX_PIN;   // from MEGA 18 (TX1)
    static const uint8_t TX_PIN;   // to   MEGA 19 (RX1)
    static const uint8_t SD_CS;   // Ethernet shield microSD CS
    File logFile;
    bool ready;

  public:
    void SD_UNO(static const uint8_t rx, static const uint8_t tx, static const uint8_t sdcs, File log): RX_PIN(rx), TX_PIN(tx), SD_CS(sdcs), logFile(log), ready(true) {}
    void ensureHeader() 
    {
      if (logFile && logFile.size() == 0) 
      {
        logFile.println(F("millis,Load,Pressure_psi"));
        logFile.flush();
      } 
    }
    void stopLogging() 
    {
      if (!ready) {return;}
      ready = false;

      link.println(F("STOP"));

      logFile.flush();
      logFile.close();

      digitalWrite(LED, HIGH);
      Serial.println(F("UNO: Logging stopped. File closed. SAFE to remove microSD / unplug USB."));
    }
    void SETUP()
    {
      if (!SD.begin(SD_CS)) 
      {
        Serial.println(F("[ERR] SD.begin failed. Insert FAT32 card before power."));
        while (1) { digitalWrite(LED, HIGH); delay(150); digitalWrite(LED, LOW); delay(150); }
      }
      logFile = SD.open(LOGNAME, FILE_WRITE);
      if (!logFile) 
      {
        Serial.println(F("[ERR] Could not open data.csv"));
        while (1) { digitalWrite(LED, HIGH); delay(400); digitalWrite(LED, LOW); delay(400); }
      }
      ensureHeader(logFile);
      Serial.println(F("UNO: Logging started. Press Enter (newline) to STOP safely."));
    }
    void WritetoSD(string lineBuf)
    {   
      if (Serial.available()) 
      {
        char c = (char)Serial.read();
        if (c == '\n') {stopLogging();}
      }
      if (loggingEnabled) 
      {
        while (link.available()) 
        {
          char c = (char)link.read();
          if (c == '\r') continue;
          if (c == '\n') 
          {
            if (lineBuf.length() > 0) 
            {
              logFile.println(lineBuf);
              logFile.flush();
              Serial.print(F("[LOG] ")); Serial.println(lineBuf);
              lineBuf = "";
            }
          } else 
          {
            if (lineBuf.length() < 200) {lineBuf += c;}
          }
        }
      } 
      else 
      {
        while (link.available()) (void)link.read();
        delay(20);
      }
    }
};
