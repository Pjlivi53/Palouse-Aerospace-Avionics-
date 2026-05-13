Palouse-Aerospace-Avionics, 2026 -2027

HalfCat Sphinx DAQ logic is reusable for the KingCrimson project, however the following things will need to be changed:
Calibration equations and factors (mainly because we will be using different measurement devices and that we are operating on MCUs that run on 3.3v logic)
The addition of thermo couples
Communication Routines (we will be using the Radio Master Nomad LoRa Radios instead of the REYAK LoRa Modules)
SD writing routine will need to be changed aswell since the Teensy 4.1 can be used for local sotrage (no need for serial comms to a seperate MCU just to save data)


