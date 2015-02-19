#ifndef _FRDIGE_H
#define _FRIDGE_H

#include <Arduino.h>


// Constants
#define TEMP_FILT_COEF .05  // Will change slowly
#define PELT_FILT_COEF .2   // May change quickly

// Setpoint values (Celcius)
#define SET_TEMP 1.6
#define MAX_PELT_TEMP 75.0
#define SAFE_PELT_TEMP 25.0
#define MAX_COOL_PWM 100.0


// Outputs
#define PELT_FAN 4         // D
#define PWM_PIN 5          // A

// Inputs
#define FRIDGE_THERM 0     // A
#define PELT_THERM 1       // A


// Monster cooling fridge
class CFridge {
public:
  CFridge();
  ~CFridge();
  boolean control_fridge(int SS, int P, int I, int D);

private:
  float setTemp;
  float peltTemp[2];
  float fridgeTemp[2];
  float fridgeError;
  void collect_data();
  float read_temp(int pin);
  boolean power_pelt(int pwm);
};



#endif

