#include "display.h"
#include "gps.h"
void setup() {
  // put your setup code here, to run once:
  initializeDisplay();
  enduranceLayout();
  // Serial.begin(9600);
  // Serial.println("test");
  // pinMode(LED_BUILTIN, OUTPUT);
  
}

void loop() {
  // digitalWrite(LED_BUILTIN, HIGH);
  // delay(100);
  // digitalWrite(LED_BUILTIN, LOW);
  // delay(100);
  // put your main code here, to run repeatedly:
  // toDo have simple setup screen function to walk user through setup options
  // fetch data from diffrent sources and display it to the user on screens
  //allow user to log events or enable continous logging of all data channels
  // have performance metric modes that test 0-60, 5-60, lateral acceleration...
  //... acceleration by RPM (Dyno Mode)
  // math channels that calculate MPG, Wh/mi, BSFC (MPH/S / (cc/s,G/s)) VE? (cc/s)

}

