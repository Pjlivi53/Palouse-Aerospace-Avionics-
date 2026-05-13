#include "StateMachine.h"

// INIT
void StateMachine::init() {
  currentState = SystemState::IDLE;
  outputs = Outputs{};

  stateStartTime = millis();
  stepStartTime = millis();

  Serial.println("STATE -> IDLE");
}

// PUBLIC ACCESSORS
const Outputs& StateMachine::getOutputs() const {
  return outputs;
}

SystemState StateMachine::getState() const {
  return currentState;
}

// MAIN UPDATE
void StateMachine::update(const Inputs& in) {
  if (currentState != SystemState::ABORT && faultDetected(in)) {
    enterState(SystemState::ABORT);
  }

  switch (currentState) {
    case SystemState::IDLE:
      runIdle(in);
      break;

    case SystemState::FILL:
      runFill(in);
      break;

    case SystemState::PRE_TEST:
      runPreTest(in);
      break;

    case SystemState::TEST:
      runTest(in);
      break;

    case SystemState::POST_TEST:
      runPostTest(in);
      break;

    case SystemState::ABORT:
      runAbort();
      break;
  }
}

// HELPERS
void StateMachine::enterState(SystemState newState) {
  currentState = newState;
  stateStartTime = millis();
  stepStartTime = millis();

  Serial.print("STATE -> ");

  switch (newState) {
    case SystemState::IDLE: Serial.println("IDLE"); break;
    case SystemState::FILL: Serial.println("FILL"); break;
    case SystemState::PRE_TEST: Serial.println("PRE_TEST"); break;
    case SystemState::TEST: Serial.println("TEST"); break;
    case SystemState::POST_TEST: Serial.println("POST_TEST"); break;
    case SystemState::ABORT: Serial.println("ABORT"); break;
  }
}

bool StateMachine::elapsed(unsigned long ms) {
  return millis() - stepStartTime >= ms;
}

bool StateMachine::faultDetected(const Inputs& in) {
  if (in.estop) return true;
  if (!in.telemetryHealthy) return true;

  return false;
}

// STATE HANDLERS
void StateMachine::runIdle(const Inputs& in) {
  outputs = Outputs{};

  if (in.startFillButton) {
    enterState(SystemState::FILL);
  }
}

// FILL
void StateMachine::runFill(const Inputs& in) {
  outputs.opv110 = false;

  if (in.operatorBackButton) {
    outputs.opv106 = false;
    outputs.opv109 = true;
    stepStartTime = millis();

    Serial.println("FILL: operator safe, OPV106 closed, OPV109 open");
  }

  if (elapsed(2000)) {
    outputs.opv104 = true;
  }

  if (in.finishFillButton) {
    outputs.opv104 = false;
    outputs.opv109 = false;
    outputs.opv106 = true;

    enterState(SystemState::PRE_TEST);
  }
}

// PRE TEST
void StateMachine::runPreTest(const Inputs& in) {
  outputs.opv118 = false;
  outputs.fpv203 = false;
  outputs.nmv305 = true;

  outputs.opv121 = true;

  if (abs(in.npt303 - in.fpt202) <= 5.0f) {
    outputs.opv110 = true;
  }

  if (in.launchButton) {
    outputs.opv110 = false;
    outputs.fpv204 = true;

    enterState(SystemState::TEST);
  }
}

// TEST
void StateMachine::runTest(const Inputs& in) {
  outputs.ignitor = true;

  if (elapsed(1000)) {
    outputs.fpv205 = true;
    outputs.opv120 = true;
  }

  if (elapsed(4000)) {
    outputs.opv120 = false;
    outputs.fpv204 = false;
    outputs.npv315 = true;
    outputs.nmv305 = false;
    outputs.opv118 = true;
    outputs.fpv203 = true;
  }

  if (elapsed(10000)) {
    outputs.fpv205 = false;
  }

  if (in.endTestButton) {
    outputs.ignitor = false;
    enterState(SystemState::POST_TEST);
  }
}

// POST TEST
void StateMachine::runPostTest(const Inputs& in) {
  outputs.ignitor = false;
  outputs.opv120 = false;
  outputs.fpv205 = false;

  outputs.opv118 = true;
  outputs.fpv203 = true;
  outputs.npv315 = true;
}

// ABORT
void StateMachine::runAbort() {
  outputs = Outputs{};

  outputs.ignitor = false;

  outputs.opv110 = false;
  outputs.opv106 = false;
  outputs.opv109 = false;
  outputs.opv104 = false;
  outputs.opv120 = false;
  outputs.fpv204 = false;
  outputs.fpv205 = false;

  outputs.opv118 = true;
  outputs.fpv203 = true;
  outputs.npv315 = true;
}