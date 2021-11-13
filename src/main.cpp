#include "display.h"
#include "gps.h"
#include <U8g2lib.h>

unsigned long lastMphUpdate;
unsigned long lastRpmUpdate;
unsigned long lastCTempUpdate;
unsigned long lastETempUpdate;
unsigned long lastVoltUpdate;
unsigned long lastFuelUpdate;
unsigned long lastLapTimeUpdate;
unsigned long lapTimeStart;
unsigned long lastGearUpdate;
// unsigned long lastTimeUpdate;
int mph=0;
int rpm=0;
int cutoff = 6000;
int lapTime[3] = {0,0,0};
int coolantTemp = 90;
int engineTemp = 90;
int fuelLevel = 100;
double battVoltage = 11.5;
String gears[6] = {"N","1","2","3","4","5"};
int gIndex = 0;
int gearDir = 1;

void setup() {
  // put your setup code here, to run once:
  initializeDisplay();
  // drawBoxGauge(8000,12000,cutoff);
  // circularGaugeLayout();
  Serial.begin(9600);
  Serial.println("test");
  // Serial.println(modf(10.51234512,1.0);
  pinMode(LED_BUILTIN,OUTPUT);
  lastMphUpdate = 0;
  lastRpmUpdate = 0;
  lastCTempUpdate = 0;
  lastETempUpdate = 0;
  lastVoltUpdate = 0;
  lastFuelUpdate = 0;
  lastLapTimeUpdate = 0;
  lastGearUpdate = 0;
  lapTimeStart = millis();
  drawBackground2();
  drawBoxGauge(rpm, 12000, cutoff);
  drawGear(gears[gIndex]);
  drawCoolantTemp(coolantTemp);
  drawEngineTemp(engineTemp);
  drawFuel(fuelLevel);
  drawVoltage(battVoltage);
}

void loop() {

  if (rpm==12000) {
    rpm = 0;
  } else {
    rpm+=100;    
  }
  drawBoxGauge(rpm, 12000,cutoff);
  
  if (millis()-lastMphUpdate>500){
    if (mph==99) {
      mph = 0;
    } else {
      mph++;
    }
    drawMph(mph);
    lastMphUpdate = millis();
  }

  if (millis()-lastLapTimeUpdate>=1000){
    int seconds = millis()/1000;
    lapTime[2] = seconds%60;
    lapTime[1] = (seconds/60)%60;
    lapTime[0] = seconds/3600;
    drawLapTime(lapTime);
    lastLapTimeUpdate = millis();
  }

  if (millis()-lastFuelUpdate>3000){
    if (fuelLevel==0) {
      fuelLevel = 100;
    } else {
      fuelLevel--;
    }
    drawFuel(fuelLevel);
    lastFuelUpdate = millis();
  }

  if (millis()-lastVoltUpdate>1000){
    if (battVoltage<10.0) {
      battVoltage = 12.0;
    } else {
      battVoltage=battVoltage-.1;
    }
    drawVoltage(battVoltage);
    lastVoltUpdate = millis();
  }

  if (millis()-lastCTempUpdate>4000){
    if (coolantTemp==120) {
      coolantTemp = 90;
    } else {
      coolantTemp++;
    }
    drawCoolantTemp(coolantTemp);
    lastCTempUpdate = millis();
  }

  if (millis()-lastETempUpdate>4500){
    if (engineTemp==120) {
      engineTemp = 90;
    } else {
      engineTemp++;
    }
    drawEngineTemp(coolantTemp);
    lastETempUpdate = millis();
  }

  if (millis()-lastGearUpdate>6000){
    if (gIndex<=0) {
      gearDir = 1;
    } else if (gIndex>=5) {
      gearDir = -1;
    }
    drawGear(gears[gIndex]);
    gIndex+=gearDir;
    lastGearUpdate = millis();
  }


  // put your main code here, to run repeatedly:
  // toDo have simple setup screen function to walk user through setup options
  // fetch data from diffrent sources and display it to the user on screens
  //allow user to log events or enable continous logging of all data channels
  // have performance metric modes that test 0-60, 5-60, lateral acceleration...
  //... acceleration by RPM (Dyno Mode)
  // math channels that calculate MPG, Wh/mi, BSFC (MPH/S / (cc/s,G/s)) VE? (cc/s)

}

