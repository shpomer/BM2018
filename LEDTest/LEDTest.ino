/*  
  This code is for Teensy 3.5 (LED Controller)
  53 LEDs per palm frond.
  6 fronds per tree.
  TOTAL: 318 per tree top

  The circles on each trunk are as follows:

  27 LEDS on top
  29 LEDS
  31 LEDS
  33 LEDS
  35 LEDS
  37 LEDS
  39 LEDS
  41 LEDS on bottom

  272 per tree trunk

  Required Connections
  --------------------
    pin 2:  LED Strip #1    OctoWS2811 drives 8 LED Strips.
    pin 14: LED strip #2    All 8 are the same length.
    pin 7:  LED strip #3
    pin 8:  LED strip #4    A 100 ohm resistor should used
    pin 6:  LED strip #5    between each Teensy pin and the
    pin 20: LED strip #6    wire to the LED strip, to minimize
    pin 21: LED strip #7    high frequency ringining & noise.
    pin 5:  LED strip #8
    pin 15 & 16 - Connect together, but do not use
    pin 4 - Do not use
    pin 3 - Do not use as PWM.  Normal use is ok.

  OCTOWS2811 connections
  ----------------------
  first jack - 
  Orange: LED strip pin 5 data (right trunk )
  Blue: Led strip pin 6 data (right leaves)
  Brown: Led strip pin 8 data (left trunk)
                                              
  second jack - 
  Orange: Led strip pin 7 data (left leaves)

  TX (pin D3) from Teensy 2.0 (controller) connects to RX1 (pin 1) on Teensy 3.5
  RX (D2) from Teensy 2.0 (controller) connects to TX1 (pin 2) on Teensy 3.5

  This test is useful for checking if your LED strips work, and which
  color config (WS2811_RGB, WS2811_GRB, etc) they require.


  TO DO:
  figure out how order of LEDs works from teensy adapter
*/

#include <OctoWS2811.h>

const int ledsPerStrip = 318;
const int numStrips = 4;

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

void setup() {
  leds.begin();
  leds.show();
  Serial1.begin(9600);
  Serial.begin(9600);
}
// More intense...
/*
#define RED    0xFF0000
#define GREEN  0x00FF00
#define BLUE   0x0000FF
#define YELLOW 0xFFFF00
#define PINK   0xFF1088
#define ORANGE 0xE05800
#define WHITE  0xFFFFFF
*/

#define RED    0x160000
#define GREEN  0x001600
#define BLUE   0x000016
#define YELLOW 0x101400
#define PINK   0x120009
#define ORANGE 0x100400
#define WHITE  0x101010

elapsedMillis frameTime;

//make sure the animations don't start unless the buttons are pushed
int cmdstate1 = 100;
int cmdstate2 = 100;
int cmdstate3 = 100;
int cmdstate4 = 100;
int cmdstate5 = 100;
float background_hue = 89.0; //default to green

void loop() {
  checkSerial();

  // draw color changing background
  colorBackground();

  int microsec = 2000000 / leds.numPixels();  // change them all in 2 seconds

  if (vstate1 < 35) {
    colorWipe(RED, microsec);
    vstate1++;
  }

  if (vstate2 < 35) {
    colorWipe(BLUE, microsec);
    vstate2++;
  }

  if (vstate3 < 35) {
    colorWipe(PINK, microsec);
    vstate3++;
  }

  if (vstate4 < 35) {
    colorTrunk(GREEN, microsec);
    vstate4++;
  }

  if (vstate5 < 35) {
    colorTrunk(WHITE, microsec);
    vstate5++;
  }

}


void checkSerial() {
  // uncomment for voltage controlled speed
  // millisec = analogRead(A9) / 40;
  if (Serial1.available()) {
    Serial.println("serial is available");
    byte cmd = Serial1.read();
    Serial.println(cmd);

    if (cmd == 1) { //commands are named for the button pin number on Teensy 2.0
      cmdstate1 = 0;
    }

    else if (cmd == 3) {
      cmdstate2 = 0;
    }

    else if (cmd == 4) { 
      cmdstate3 = 0;
    }

    else if (cmd == 20) {
      cmdstate4 = 0;
    }

    else if (cmd == 2) {
      cmdstate5 = 0;
    }

    else {
      colorWipe(WHITE, microsec);
      colorWipe(GREEN, microsec);
      colorWipe(BLUE, microsec);
    }
}

void colorWipe(int color, int wait)
{
  for (int i=0; i < leds.numPixels(); i++) {
    leds.setPixel(i, color);
    leds.show();
    delayMicroseconds(wait);
  }
}
void colorTrunk(int color, int wait)
{
  for (int i=0; i < 272; i++) {
    int trunk_band = 
    for (int trunk_band=41; trunk_band >= 27; trunk_band-=2) {
      leds.setPixel(trunk_band, color); //how do I set multiple pixels at once?
      leds.show();
    }
    delayMicroseconds(wait);
  }
}
void colorFronds(int color, int wait)
{
  for (int i=0; i < ledsPerStrip; i++) {
    for (int frond_leaf=6; frond_leaf >= 4; frond_leaf--) {
      leds.setPixel(i + trunk_band, color); 
      leds.show();
    }

    delayMicroseconds(wait);
  }
}

void colorBackground() {
  float deltahue = ((float)((int)random(2000) - 1000)) * 0.003; // -2.0 to +2.0
  deltahue += (background_hue - background_hue) * 0.001; // bias towards 269.0
  //Serial.printf("deltahue = %.2f\n", deltahue);
  background_hue += deltahue;
  if (background_hue > 289.0) background_hue = 289.0; //don't get too red
  if (background_hue < 80.0) background_hue = 80.0; //don't get too yellow-green
  unsigned int hue = background_hue;
  //Serial.printf("hue = %d\n", hue);

  for (int i = 0; i < ledsPerStrip; i++) { //set color on right trunk
    unsigned int saturation = 100 + random(25);
    unsigned int lightness = 30 + random(10);
    leds.setPixel(i, makeColor(hue - (i * 0.6), saturation, lightness));
  }
  for (int i = ledsPerStrip; i < ledsPerStrip * 2; i++) { //set color on right fronds
    unsigned int saturation = 100 + random(25); 
    unsigned int lightness = 30 + random(10);
    leds.setPixel(i, makeColor(hue - ((i - ledsPerStrip) * 0.6), saturation, lightness));
  }
  for (int i = ledsPerStrip; i < ledsPerStrip * 2; i++) { //set color on left fronds
    unsigned int saturation = 100 + random(25); 
    unsigned int lightness = 30 + random(10);
    leds.setPixel(i, makeColor(hue - ((i - ledsPerStrip) * 0.6), saturation, lightness));
  }
  for (int i = ledsPerStrip; i < ledsPerStrip * 2; i++) { //set color on left trunk
    unsigned int saturation = 100 + random(25); 
    unsigned int lightness = 30 + random(10);
    leds.setPixel(i, makeColor(hue - ((i - ledsPerStrip) * 0.6), saturation, lightness));
  }
}
