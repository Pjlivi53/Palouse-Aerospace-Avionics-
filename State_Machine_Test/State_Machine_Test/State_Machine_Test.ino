#include "StateMachine.h"

StateMachine sm;
Inputs in;

// PIN MAP
const int PIN_IGNITOR = 10;

const int PIN_OPV110 = 11;
const int PIN_OPV106 = 12;
const int PIN_OPV109 = 13;
const int PIN_OPV104 = 14;
const int PIN_OPV118 = 15;
const int PIN_OPV120 = 16;
const int PIN_OPV121 = 17;

const int PIN_FPV203 = 18;
const int PIN_FPV204 = 19;
const int PIN_FPV205 = 20;

const int PIN_NMV305 = 21;
const int PIN_NPV315 = 22;

// OUTPUTS
void setupPins() {
  pinMode(PIN_IGNITOR, OUTPUT);

  pinMode(PIN_OPV110, OUTPUT);
  pinMode(PIN_OPV106, OUTPUT);
  pinMode(PIN_OPV109, OUTPUT);
  pinMode(PIN_OPV104, OUTPUT);
  pinMode(PIN_OPV118, OUTPUT);
  pinMode(PIN_OPV120, OUTPUT);
  pinMode(PIN_OPV121, OUTPUT);

  pinMode(PIN_FPV203, OUTPUT);
  pinMode(PIN_FPV204, OUTPUT);
  pinMode(PIN_FPV205, OUTPUT);

  pinMode(PIN_NMV305, OUTPUT);
  pinMode(PIN_NPV315, OUTPUT);
}

void applyOutputs(const Outputs& o) {
  digitalWrite(PIN_IGNITOR, o.ignitor);

  digitalWrite(PIN_OPV110, o.opv110);
  digitalWrite(PIN_OPV106, o.opv106);
  digitalWrite(PIN_OPV109, o.opv109);
  digitalWrite(PIN_OPV104, o.opv104);
  digitalWrite(PIN_OPV118, o.opv118);
  digitalWrite(PIN_OPV120, o.opv120);
  digitalWrite(PIN_OPV121, o.opv121);

  digitalWrite(PIN_FPV203, o.fpv203);
  digitalWrite(PIN_FPV204, o.fpv204);
  digitalWrite(PIN_FPV205, o.fpv205);

  digitalWrite(PIN_NMV305, o.nmv305);
  digitalWrite(PIN_NPV315, o.npv315);
}

// KEYBOARD INPUT
void resetMomentaryInputs() {
  in.startFillButton = false;
  in.operatorBackButton = false;
  in.finishFillButton = false;
  in.launchButton = false;
  in.endTestButton = false;
}

void handleKeyboard() {
  resetMomentaryInputs();

  if (!Serial.available()) return;

  char c = Serial.read();

  if (c == '\n' || c == '\r') return;

  switch (c) {
    case 'f':
      in.startFillButton = true;
      Serial.println("KEY: Start Fill");
      break;

    case 'b':
      in.operatorBackButton = true;
      Serial.println("KEY: Operator Back / Safe");
      break;

    case 'n':
      in.finishFillButton = true;
      Serial.println("KEY: Finish Fill");
      break;

    case 'l':
      in.launchButton = true;
      Serial.println("KEY: Launch");
      break;

    case 'e':
      in.endTestButton = true;
      Serial.println("KEY: End Test");
      break;

    case 'x':
      in.estop = true;
      Serial.println("KEY: ESTOP");
      break;

    case 'r':
      in.estop = false;
      Serial.println("KEY: Reset ESTOP");
      sm.init();
      break;

    case 'p':
      printOutputs(sm.getOutputs());
      break;

    case 'h':
      printHelp();
      break;

    default:
      Serial.print("Unknown key: ");
      Serial.println(c);
      Serial.println("Type h for help.");
      break;
  }
}

// DEBUG
void printHelp() {
  Serial.println();
  Serial.println("===== KEYBOARD CONTROLS =====");
  Serial.println("f = Start FILL");
  Serial.println("b = Operator back / safe confirmation");
  Serial.println("n = Finish FILL");
  Serial.println("l = Launch / enter TEST");
  Serial.println("e = End TEST");
  Serial.println("x = ESTOP / ABORT");
  Serial.println("r = Reset ESTOP and return to IDLE");
  Serial.println("p = Print outputs");
  Serial.println("h = Help");
  Serial.println("=============================");
  Serial.println();
}

void printOutputs(const Outputs& o) {
  Serial.print("IGN:"); Serial.print(o.ignitor);

  Serial.print(" OPV110:"); Serial.print(o.opv110);
  Serial.print(" OPV106:"); Serial.print(o.opv106);
  Serial.print(" OPV109:"); Serial.print(o.opv109);
  Serial.print(" OPV104:"); Serial.print(o.opv104);
  Serial.print(" OPV118:"); Serial.print(o.opv118);
  Serial.print(" OPV120:"); Serial.print(o.opv120);
  Serial.print(" OPV121:"); Serial.print(o.opv121);

  Serial.print(" FPV203:"); Serial.print(o.fpv203);
  Serial.print(" FPV204:"); Serial.print(o.fpv204);
  Serial.print(" FPV205:"); Serial.print(o.fpv205);

  Serial.print(" NMV305:"); Serial.print(o.nmv305);
  Serial.print(" NPV315:"); Serial.print(o.npv315);

  Serial.println();
}

// ARDUINO SETUP / LOOP
void setup() {
  Serial.begin(115200);
  delay(1000);

  setupPins();

  in.telemetryHealthy = true;

  sm.init();

  Serial.println("State machine keyboard test ready.");
  printHelp();
}

void loop() {
  handleKeyboard();

  sm.update(in);

  applyOutputs(sm.getOutputs());

  static unsigned long lastPrint = 0;
  if (millis() - lastPrint >= 1000) {
    printOutputs(sm.getOutputs());
    lastPrint = millis();
  }
}