#include "Fridge.h"



// Constructor
CFridge::CFridge()
{
  // Use ADC voltage reference of 1.1V for higher resolution
  analogReference(INTERNAL);
  
  // Set desired temp of fridge
  setTemp = SET_TEMP;
  
  // Initialize current temp of peltier plate
  peltTemp[0] = read_temp(PELT_THERM);
  
  // Initialize current temp of fridge
  fridgeTemp[0] = read_temp(FRIDGE_THERM);
}



// Destructor
CFridge::~CFridge() {}



// Calculate control response for fridge
boolean CFridge::control_fridge(int SS, int P, int I, int D)
{
  // Collect current data
  collect_data();
  
  // Calculate control response and force upper and lower limits
  float ctrlResponse = (float)SS + (float)P*fridgeError + (float)D*(fridgeTemp[1]-fridgeTemp[0]);
  if (ctrlResponse > 255)
    ctrlResponse = 255;
  else if (ctrlResponse < 0)
    ctrlResponse = 0;
  
  // Attempt to send control response to the peltier plate, return successful or not
  return(power_pelt(ctrlResponse));
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// Read fridge temp and calculate error
void CFridge::collect_data()
{
  // Shift data, measure and filter peltier temp
  peltTemp[1] = peltTemp[0];
  peltTemp[0] = (PELT_FILT_COEF * read_temp(PELT_THERM)) + ((1-PELT_FILT_COEF) * peltTemp[1]);
  
  // Shift data, measure and filter fridge temp
  fridgeTemp[1] = fridgeTemp[0];
  fridgeTemp[0] = (TEMP_FILT_COEF * read_temp(FRIDGE_THERM)) + ((1-TEMP_FILT_COEF) * fridgeTemp[1]);
  
  // Calculate fridge error (current - last)
  fridgeError = fridgeTemp[0] - SET_TEMP;
}



// Read temperature (C) from an analog pin
float CFridge::read_temp(int pin)
{
  // Read and convert to Celcius using 1.1V ADC reference
  return((float)analogRead(pin) / 9.31);
}



// Send PWM signal to peltier plate
boolean CFridge::power_pelt(int pwm)
{
  boolean success = true;
  
  // Check for overheat condition
  if (peltTemp[0] >= MAX_PELT_TEMP)
  {
    // Completely turn off peltier plate and turn on fan
    pinMode(PWM_PIN, OUTPUT);
    digitalWrite(PWM_PIN, LOW);
    digitalWrite(PELT_FAN, HIGH);
    success = false;  // Unsuccessful setting of peltier PWM
  }
  // If peltier is cool enough (with hysteresis of 3degC)
  else if (peltTemp[0] <= (MAX_PELT_TEMP - 3.0))
  {
    // Send PWM signal
    analogWrite(PWM_PIN, pwm);
    
    // Check to see if fan should be on or off (hysteresis of 10degC)
    if (peltTemp[0] >= (SAFE_PELT_TEMP + 10.0))
      digitalWrite(PELT_FAN, HIGH);
    else if (peltTemp[0] <= SAFE_PELT_TEMP)
      digitalWrite(PELT_FAN, LOW);
  }
  
  return(success);
}
