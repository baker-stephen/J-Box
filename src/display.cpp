#include "display.h"
#include <string>
#include <math.h>

int screenx = 240;
int screeny = 128;
int fontx = 12;
int fonty = 15;


U8G2_T6963_240X128_F_8080 u8g2(U8G2_R2, 2, 14, 7, 8, 6, 20, 21, 5, /*enable/wr=*/ 27 , /*cs/ce=*/ 26, /*dc=*/ 25, /*reset=*/24); // Connect RD (orange) with +5V, FS0 and FS1 with GND
TSPoint p;
void modifyPointToScreen()
{
p.x =  map(p.x,100,944,0,240);
p.y =  map(p.y,190,860,0,128);
p.z = abs(map(p.z,900,300,0,255));
}
TouchScreen ts = TouchScreen(XP, YP, XM, YM , 730);
enum screens {menu,settings,gps,naughtTo60Timer};
screens currentScreen = settings;

void initializeDisplay()
{
    pinMode(backlightPin,OUTPUT);
    analogWrite(backlightPin,100);
    u8g2.begin();
    //u8g2.setFlipMode(0);
    u8g2.setContrast(255);
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(32,20,"Booting Up");
    u8g2.sendBuffer();
}

void menuScreen()
{
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_VCR_OSD_mf); //u8g2_font_9x15B_mr
    u8g2.drawStr(32,18,"Menu");
    u8g2.drawLine(0,19,240,19);
    ///u8g2.setFontMode(0); // shoulnt need to change this as it is the default
    u8g2.setDrawColor(0); // set this to zero so that we get a black background against the font
    u8g2.drawStr(0,40,"Device Settings");
    u8g2.drawStr(0,60,"");
    u8g2.drawStr(0,80,"Acceleration Tests");
    

    
    
    
    u8g2.sendBuffer();
    u8g2.setDrawColor(1); // reset the draw color back to the default




}

// Draws a portion of a cicle, angle in radians. Keep in mind y axis is flipped.
void drawPartialCircle(int xo, int yo, double radius, double initialAngle, double finalAngle){
    for (double angle = initialAngle; angle<=finalAngle; angle = angle + .01){
        u8g2.drawPixel(xo+int(radius*cos(angle)),yo+int(radius*sin(angle)));
    }
}

//Draws a circular gauge with a "needle" at some proportion of a maximum
void drawGauge(int xo, int yo, int rad, double maxValue, double currentValue) {
    double initAngle = M_PI*(1.0/4.0);
    double finalAngle = M_PI*(7.0/4.0);
    drawPartialCircle(xo,yo,rad*1.0,initAngle,finalAngle);
    double angle = (currentValue/maxValue)*(finalAngle-initAngle)+initAngle;
    //needle 3 pixels thick
    for (int i = -1; i<=1; i++) {
        for (int j = -1; j<=1; j++) {
            u8g2.drawLine(xo+i,yo+j,xo+int(rad*cos(angle))+i,yo+int(rad*sin(angle))+j);
        }
    }
    // u8g2.drawLine(xo-1,yo-1,xo+int(rad*cos(angle))-1,yo+int(rad*sin(angle))-1);
    // u8g2.drawLine(xo,yo,xo+int(rad*cos(angle)),yo+int(rad*sin(angle)));
    // u8g2.drawLine(xo+1,yo+1,xo+int(rad*cos(angle))+1,yo+int(rad*sin(angle))+1);
    //markers and labels
    u8g2.setFont(u8g2_font_chroma48medium8_8n);
    int i = 0;
    int numNotches = maxValue/1000;
    for (double angle = initAngle; angle<=finalAngle; angle = angle + (finalAngle-initAngle)/numNotches){
        u8g2.drawLine(xo+int(rad*.85*cos(angle)),yo+int(rad*.85*sin(angle)),xo+int(rad*cos(angle)),yo+int(rad*sin(angle)));
        u8g2.drawStr(xo+int(rad*.85*cos(angle)-8.0/sqrt(2)),yo+int(rad*.85*sin(angle)+8.0/sqrt(2)),String(i).c_str());
        i = i+1;
    }
}

void drawCircularBarGauge(int xo, int yo, int rad, double maxValue, double currentValue) {
    double initAngle = M_PI*(1.0/4.0);
    double finalAngle = M_PI*(7.0/4.0);
    double currentAngle = (currentValue/maxValue)*(finalAngle-initAngle)+initAngle;
    for (double angle = initAngle; angle<=currentAngle; angle = angle + .001){
        double lin = (currentValue / maxValue) / 2.0;
        u8g2.drawLine(xo+int(rad*lin*cos(angle)),yo+int(rad*lin*sin(angle)),xo+int(rad*cos(angle)),yo+int(rad*sin(angle)));
    }
}

void drawGear(String gear){
    u8g2.setFont(u8g2_font_logisoso58_tf);
    u8g2.drawStr(screenx/3,screeny/2+31,gear.c_str());
    u8g2.drawFrame(screenx/3-4,screeny/2-32,46,68);
}

void drawMph(int mph) {
    u8g2.setDrawColor(0);
    u8g2.drawBox(0,0,40,31);
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_logisoso30_tr);
    if (mph<10) {
        u8g2.drawStr(20,30,String(mph).c_str());
    } else {
        u8g2.drawStr(0,30,String(mph).c_str());
    }
    u8g2.sendBuffer();
}

void drawRpm(int rpm) {
    u8g2.setDrawColor(0);
    u8g2.drawBox(110,0,150,31);
    u8g2.drawBox(150,0,fontx*3,fonty*2+1);
    u8g2.drawDisc(screenx-55,screeny/2,46);
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_logisoso30_tr);
    if (rpm<10000) {
        u8g2.drawStr(130,30,String(rpm/1000).c_str());
    } else {
        u8g2.drawStr(110,30,String(rpm/1000).c_str());
    }
    u8g2.setFont(u8g2_font_VCR_OSD_mf);
    u8g2.drawStr(150,fonty,String(rpm%1000).c_str());
    drawGauge(screenx-55,screeny/2,45,12000.0,rpm*1.0);
    u8g2.sendBuffer();
}

void drawCoolantTemp(int coolantTemp) {
    u8g2.setFont(u8g2_font_VCR_OSD_mf);
    u8g2.drawStr(0,screeny-fonty,String(coolantTemp).c_str());
}
void drawEngineTemp(int engineTemp) {
    u8g2.setFont(u8g2_font_VCR_OSD_mf);
    u8g2.drawStr(0,screeny,String(engineTemp).c_str());
}

void drawFuel(int fuelLevel) {
    u8g2.setFont(u8g2_font_VCR_OSD_mf);
    if (fuelLevel>=100){
        u8g2.drawStr((screenx/2)-fontx*(7/2),screeny,String(fuelLevel).c_str());
    } else if (fuelLevel>10) {
        u8g2.drawStr((screenx/2)-fontx*(5/2),screeny,String(fuelLevel).c_str());
    } else {
        u8g2.drawStr((screenx/2)-fontx*(3/2),screeny,String(fuelLevel).c_str());
    }
}

void drawLapTime(int *lapTime){
    u8g2.setFont(u8g2_font_VCR_OSD_mf);
    if (lapTime[0]<10) {
        u8g2.drawStr(screenx-fontx*7,screeny,String(lapTime[0]).c_str());
    } else {
        u8g2.drawStr(screenx-fontx*8,screeny,String(lapTime[0]).c_str());
    }
    if (lapTime[1]<10) {
        u8g2.drawStr(screenx-fontx*5,screeny,"0");
        u8g2.drawStr(screenx-fontx*4,screeny,String(lapTime[1]).c_str());
    }
    else {
        u8g2.drawStr(screenx-fontx*5,screeny,String(lapTime[1]).c_str());
    }
    if (lapTime[2]<10) {
        u8g2.drawStr(screenx-fontx*2,screeny,"0");
        u8g2.drawStr(screenx-fontx*1,screeny,String(lapTime[2]).c_str());
    }
    else {
        u8g2.drawStr(screenx-fontx*2,screeny,String(lapTime[2]).c_str());
    }
}

void drawVoltage(double battVoltage) {
    u8g2.setFont(u8g2_font_VCR_OSD_mf);
    u8g2.drawStr(0,screeny/2+fonty/2,String(int(battVoltage)).c_str());
    u8g2.drawStr(fontx*2,screeny/2+fonty/2,".");
    int firstDecimal = int((battVoltage-int(battVoltage))*10 + 0.5);
    u8g2.drawStr(fontx*3,screeny/2+fonty/2,String(firstDecimal).c_str());
}

void drawBackground() {
    int screenx = 240;
    int screeny = 128;
    u8g2.setFont(u8g2_font_VCR_OSD_mf);
    int fontx = 12;
    int fonty = 15;

    u8g2.drawFrame(screenx/3-4,screeny/2-32,46,68);
    
    u8g2.drawStr(40,fonty,"mph");
    
    u8g2.drawStr(screenx - fontx*3,fonty,"rpm");

    u8g2.drawStr(fontx*3,screeny-fonty,"C");

    u8g2.drawStr(fontx*3,screeny,"C");
    
    u8g2.drawStr((screenx/2),screeny,"%");

    u8g2.drawStr(screenx-fontx*6,screeny,":");

    u8g2.drawStr(screenx-fontx*3,screeny,":");

    u8g2.drawStr(fontx*4,screeny/2+fonty/2,"V");
}

void circularGaugeLayout() {
    int mph = 69;
    String gear = "3";
    int rpm = 11420;
    int coolantTemp = 109;
    int engineTemp = 120;
    int fuelLevel = 42;
    int lapTime[3] = {1,23,45};
    double battVoltage = 11.572345;
    

    u8g2.clearBuffer();
    
    drawBackground();

    
    drawGear(gear);
    drawMph(mph);
    drawRpm(rpm);
    drawCoolantTemp(coolantTemp);
    drawEngineTemp(engineTemp);
    drawFuel(fuelLevel);
    drawLapTime(lapTime);
    drawVoltage(battVoltage);
    u8g2.sendBuffer();
}

void drawBackground2() {

    int screenx = 240;
    int screeny = 128;
    u8g2.setFont(u8g2_font_VCR_OSD_mf);
    int fontx = 12;
    int fonty = 15;

    u8g2.drawFrame(screenx/3-4,screeny/2-32,46,68);
    
    u8g2.drawStr(40,fonty,"mph");
    
    u8g2.drawStr(screenx - fontx*3,fonty,"rpm");

    u8g2.drawStr(fontx*3,screeny-fonty,"C");

    u8g2.drawStr(fontx*3,screeny,"C");
    
    u8g2.drawStr((screenx/2),screeny,"%");

    u8g2.drawStr(screenx-fontx*6,screeny,":");

    u8g2.drawStr(screenx-fontx*3,screeny,":");

    u8g2.drawStr(fontx*4,screeny/2+fonty/2,"V");

}

void drawBoxGauge(int current, int max) {
    u8g2.drawFrame(0,0,screenx,screeny/5);
    u8g2.drawBox(0,0,int(screenx*(float(current)/float(max))),screeny/5);
    u8g2.sendBuffer();
}

// void displayGPSbootup()
// {
//     u8g2.setCursor(32,40);
//     u8g2.print("gps not functional");

// }