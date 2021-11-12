#include "display.h"
#include "gps.h"
#include <U8g2lib.h>

unsigned long lastMphUpdate;
unsigned long lastRpmUpdate;
// unsigned long lastTimeUpdate;
int mph;
int rpm;
int cutoff = 6000;

void setup() {
  // put your setup code here, to run once:
  initializeDisplay();
  drawBoxGauge(8000,12000,cutoff);
  // circularGaugeLayout();
  Serial.begin(9600);
  Serial.println("test");
  pinMode(LED_BUILTIN,OUTPUT);
  lastMphUpdate = 0;
  lastRpmUpdate = 0;
  mph = 0;
  rpm = 8000;
  drawGear("N");
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
      rpm = 9000;
  } else {
    rpm+=10;    
  }
  drawBoxGauge(rpm, 12000,cutoff);
  // put your main code here, to run repeatedly:
  // toDo have simple setup screen function to walk user through setup options
  // fetch data from diffrent sources and display it to the user on screens
  //allow user to log events or enable continous logging of all data channels
  // have performance metric modes that test 0-60, 5-60, lateral acceleration...
  //... acceleration by RPM (Dyno Mode)
  // math channels that calculate MPG, Wh/mi, BSFC (MPH/S / (cc/s,G/s)) VE? (cc/s)

}

