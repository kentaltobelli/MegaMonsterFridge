// MegaMonster Peltier Fridge
// Kent Altobelli

#include "Fridge.h"

#define LOOP_TIME 500  // ms

#define SS_SIG 30  // Based on thermal leakage (rqd for steady state)
#define P_COEF 10
#define I_COEF 0
#define D_COEF 0


CFridge fridge;  // Instance of fridge
boolean success;


void setup()
{
  //Serial.begin(9600);
}

void loop()
{
  // Calculate and send control response to peltier to maintain temp
  success = fridge.control_fridge(SS_SIG, P_COEF, I_COEF, D_COEF);
  
  delay(LOOP_TIME);
}
