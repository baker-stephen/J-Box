#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>
#include <U8g2lib.h>
#include "TouchScreen.h"
void menuScreen();
void setupScreen();
void gpsScreen();
void circularGaugeLayout();
void naughtTo60Screen();
void initializeDisplay();
void drawMph(int mph);
void drawRpm(int rpm);
void drawBoxGauge(int current, int max, int cutoff);
void drawGear(String gear);
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A8  // must be an analog pin, use "An" notation!
#define YM A1  // can be a digital pin
#define XP A2   // can be a digital pin
const int backlightPin = 23;
#define MINPRESSURE 10
#define MAXPRESSURE 1000
#endif