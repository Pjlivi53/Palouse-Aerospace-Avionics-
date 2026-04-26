#pragma once
#include <Arduino.h>

// PUBLIC STATES
enum class SystemState {
    IDLE,
    FILL,
    PRE_TEST,
    TEST,
    POST_TEST,
    ABORT
};

// INPUTS
struct Inputs {
    bool startFillButton = false;
    bool operatorBackButton = false;
    bool finishFillButton = false;
    bool launchButton = false;
    bool endTestButton = false;
    bool estop = false;

    float otc124 = 0.0f;
    float npt303 = 0.0f;
    float fpt202 = 0.0f;

    bool telemetryHealthy = true;
};

// ==============================
// OUTPUTS
// ==============================

struct Outputs {
    bool ignitor = false;

    bool opv110 = false;
    bool opv106 = false;
    bool opv109 = false;
    bool opv104 = false;
    bool opv118 = false;
    bool opv120 = false;
    bool opv121 = false;

    bool fpv203 = false;
    bool fpv204 = false;
    bool fpv205 = false;

    bool nmv305 = false;
    bool npv315 = false;
};

// STATE MACHINE CLASS
class StateMachine {
public:
    void init();
    void update(const Inputs& in);

    const Outputs& getOutputs() const;
    SystemState getState() const;

private:
    SystemState currentState;
    Outputs outputs;

    unsigned long stateStartTime;
    unsigned long stepStartTime;

    void runIdle(const Inputs& in);
    void runFill(const Inputs& in);
    void runPreTest(const Inputs& in);
    void runTest(const Inputs& in);
    void runPostTest(const Inputs& in);
    void runAbort();

    void enterState(SystemState newState);
    bool elapsed(unsigned long ms);
    bool faultDetected(const Inputs& in);
};