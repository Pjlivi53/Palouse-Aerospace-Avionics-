

#include <SPI.h>
#include <SD.h>

static const uint8_t SD_CS  = 4;   // microSD CS on Ethernet shield
static const uint8_t ETH_CS = 10;  // Ethernet chip select (keep HIGH)
static const char*   LOGNAME = "Sphinx.csv";

File    logFile;
String  buf;
uint16_t lineCountSinceFlush = 0;
const uint16_t FLUSH_EVERY_N = 20; // set to 1 for flush every line

void ensureHeader(File &f) {
  if (f && f.size() == 0) {
    f.println(F("millis,Load,Chamber_psi,RunTank_psi"));
    f.flush();
  }
}

void setup() {
  Serial.begin(57600);            // must match MEGA Serial2
  pinMode(ETH_CS, OUTPUT);
  digitalWrite(ETH_CS, HIGH);     // keep Ethernet off the SPI bus

  SD.begin(SD_CS);                // if this fails, logging won't occur (headless)
  logFile = SD.open(LOGNAME, FILE_WRITE);
  ensureHeader(logFile);
}

void loop() {
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\r') continue;
    if (c == '\n') {
      if (buf.length() > 0) {
        if (buf == "STOP") {
          if (logFile) { logFile.flush(); logFile.close(); }
          for (;;) { delay(1000); } // halt after clean stop
        }
        if (logFile) {
          logFile.println(buf);
          if (++lineCountSinceFlush >= FLUSH_EVERY_N) {
            logFile.flush();
            lineCountSinceFlush = 0;
          }
        }
        buf = "";
      }
    } else {
      if (buf.length() < 240) buf += c;
    }
  }
}
