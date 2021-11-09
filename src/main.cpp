#include "display.h"
#include "gps.h"
#include <U8g2lib.h>

unsigned long lastMphUpdate;
unsigned long lastRpmUpdate;
// unsigned long lastTimeUpdate;
int mph;
int rpm;

void setup() {
  // put your setup code here, to run once:
  initializeDisplay();
  circularGaugeLayout();
  // Serial.begin(9600);
  // Serial.println("test");
  pinMode(LED_BUILTIN,OUTPUT);
  lastMphUpdate = 0;
  lastRpmUpdate = 0;
  mph = 0;
  rpm = 0;
}

void loop() {
  if (millis()-lastMphUpdate>500){
    if (mph==99) {
      mph = 0;
    } else {
      mph++;
    }
    drawMph(mph);
    lastMphUpdate = millis();
  }

  if (rpm==12000) {
      rpm = 0;
  } else {
    rpm+=100;    
  }
  drawRpm(rpm);
  // put your main code here, to run repeatedly:
  // toDo have simple setup screen function to walk user through setup options
  // fetch data from diffrent sources and display it to the user on screens
  //allow user to log events or enable continous logging of all data channels
  // have performance metric modes that test 0-60, 5-60, lateral acceleration...
  //... acceleration by RPM (Dyno Mode)
  // math channels that calculate MPG, Wh/mi, BSFC (MPH/S / (cc/s,G/s)) VE? (cc/s)

}

