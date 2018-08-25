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

// Convert HSL (Hue, Saturation, Lightness) to RGB (Red, Green, Blue)
//
//   hue:        0 to 359 - position on the color wheel, 0=red, 60=orange,
//                            120=yellow, 180=green, 240=blue, 300=violet
//
//   saturation: 0 to 100 - how bright or dull the color, 100=full, 0=gray
//
//   lightness:  0 to 100 - how light the color is, 100=white, 50=color, 0=black
//
int makeColor(unsigned int hue, unsigned int saturation, unsigned int lightness)
{
  unsigned int red, green, blue;
  unsigned int var1, var2;

  if (hue > 359) hue = hue % 360;
  if (saturation > 100) saturation = 100;
  if (lightness > 100) lightness = 100;

  // algorithm from: http://www.easyrgb.com/index.php?X=MATH&H=19#text19
  if (saturation == 0) {
    red = green = blue = lightness * 255 / 100;
  } else {
    if (lightness < 50) {
      var2 = lightness * (100 + saturation);
    } else {
      var2 = ((lightness + saturation) * 100) - (saturation * lightness);
    }
    var1 = lightness * 200 - var2;
    red = h2rgb(var1, var2, (hue < 240) ? hue + 120 : hue - 240) * 255 / 600000;
    green = h2rgb(var1, var2, hue) * 255 / 600000;
    blue = h2rgb(var1, var2, (hue >= 120) ? hue - 120 : hue + 240) * 255 / 600000;
  }
  return (red << 16) | (green << 8) | blue;
}

unsigned int h2rgb(unsigned int v1, unsigned int v2, unsigned int hue)
{
  if (hue < 60) return v1 * 60 + (v2 - v1) * hue;
  if (hue < 180) return v2 * 60;
  if (hue < 240) return v1 * 60 + (v2 - v1) * (240 - hue);
  return v1 * 60;
}

const int ledsPerFronds = 318;
const int ledsPerTrunk = 272;
const int ledsPerTree = 590;
const int totalLeds = 590;
const int numStrips = 4;

DMAMEM int displayMemory[totalLeds*6];
int drawingMemory[totalLeds*6];

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(totalLeds, displayMemory, drawingMemory, config);

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
  if (frameTime > 20) {
    frameTime -= 20; //update every 2ms. 35 frames x 20 ms per frame = 700

    // draw color changing background
    colorBackground();

    int microsec = 2000000 / leds.numPixels();  // change them all in 2 seconds

    if (cmdstate1 < 35) {
      zipAround(cmdstate1);
      colorFronds(RED, microsec);
    }

    if (cmdstate2 < 35) {
      colorWipe(BLUE, microsec);
      cmdstate2++;
    }

    if (cmdstate3 < 35) {
      colorWipe(PINK, microsec);
      cmdstate3++;
    }

    if (cmdstate4 < 35) {
      colorTrunk(GREEN, microsec);
      cmdstate4++;
    }

    if (cmdstate5 < 35) {
      colorTrunk(WHITE, microsec);
      cmdstate5++;
    }
  }
}

void checkSerial() {
  // uncomment for voltage controlled speed
  // millisec = analogRead(A9) / 40;
  if (Serial1.available()) {
    Serial.println("serial is available");
    byte cmd = Serial1.read();
    Serial.println(cmd);

    if (cmd == 1) { //Left btn. Commands are named for the button pin number on Teensy 2.0
      cmdstate1 = 0;
    }

    if (cmd == 3) { // middle btn
      cmdstate2 = 0;
    }

    if (cmd == 4) { //right btn
      cmdstate3 = 0;
    }

    if (cmd == 20) { //bottom left red glowing btn
      cmdstate4 = 0;
    }

    if (cmd == 2) { //bottom right yellow glowing btn
      cmdstate5 = 0;
    }
  }
}
void colorBackground() {
  float deltahue = ((float)((int)random(2000) - 1000)) * 0.003; // -2.0 to +2.0
  deltahue += (background_hue - background_hue) * 0.001; // bias towards 269.0
  //Serial.printf("deltahue = %.2f\n", deltahue);
  background_hue += deltahue;

  //ADJUST AS NEEDED
  if (background_hue > 189.0) background_hue = 189.0; //don't get too blue
  if (background_hue < 80.0) background_hue = 80.0; //don't get too yellow-green


  unsigned int trunk_hue = background_hue + 100; //make this browner
  unsigned int fronds_hue = background_hue;
  //Serial.printf("hue = %d\n", hue);

  //set color on right trunk
  for (int i = 0; i < ledsPerTrunk; i++) { 
    unsigned int saturation = 100 + random(25);
    unsigned int lightness = 30 + random(10);
    leds.setPixel(i, makeColor(trunk_hue - (i * 0.6), saturation, lightness));
  }

  //set color on right fronds
  for (int i = ledsPerTrunk; i < ledsPerTree; i++) { 
    unsigned int saturation = 100 + random(25); 
    unsigned int lightness = 30 + random(10);
    leds.setPixel(i, makeColor(fronds_hue - ((i - ledsPerTrunk) * 0.6), saturation, lightness));
  }

  //set color on left fronds
  for (int i = ledsPerTree; i < ledsPerTree + ledsPerFronds; i++) { 
    unsigned int saturation = 100 + random(25); 
    unsigned int lightness = 30 + random(10);
    leds.setPixel(i, makeColor(fronds_hue - ((i - ledsPerTree) * 0.6), saturation, lightness));
  }

  //set color on left trunk
  for (int i = (ledsPerTree + ledsPerFronds); i < totalLeds; i++) { 
    unsigned int saturation = 100 + random(25); 
    unsigned int lightness = 30 + random(10);
    leds.setPixel(i, makeColor(trunk_hue - ((i - totalLeds) * 0.6), saturation, lightness));
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

void colorTrunks(int color, int wait)
{
  for (int i = 0; i < ledsPerTrunk; i++) { 
    leds.setPixel(i, color);
  }
  for (int i = ledsPerTree + ledsPerFronds; i < totalLeds; i++) {
    leds.setPixel(i, color);
  }
  leds.show();
}

void rainbowTrunks(int color, int wait)
{
  for (int i=0; i < ledsPerTrunk; i++) {
    for (int trunk_band=41; trunk_band >= 27; trunk_band-=2) {
      leds.setPixel(trunk_band, color); //how do I set multiple pixels at once?
      leds.show();
    }
    delayMicroseconds(wait);
  }
}

void colorFronds(int color, int wait)
{
  //set color on right fronds
  for (int i = ledsPerTrunk; i < ledsPerTree + ledsPerFronds; i++) { 
    leds.setPixel(i, color);
    leds.show();
    delayMicroseconds(wait);
  }
}

void zipAround(int cmdstate) {
  //right trunk
  int color_jump = (float)cmdstate * (ledsPerTrunk / 35);
  int zip_width = color_jump + 2; 
  if (zip_width >= ledsPerTrunk) zip_width = ledsPerTrunk - 1;
  for (int i = color_jump; i < zip_width; i++) {
    leds.setPixel(i, 0xFF7800); //set zip to yellow
    leds.show(); //not sure if I need to call this or not...
  }
  cmdstate++;
}

void zipAroundTrunks(int cmdstate) {
  //right trunk
  int color_jump = (float)cmdstate * (ledsPerTrunk / 35);
  int zip_width = color_jump + 2; 
  if (zip_width >= ledsPerTrunk) {
    zip_width = ledsPerTrunk - 1;
  }
  for (int i = color_jump; i < zip_width; i++) {
    leds.setPixel(i, 0xFF7800); //set zip to yellow
    leds.setPixel((ledsPerTree + ledsPerFronds) + i, 0xFF7800); //set zip to yellow
    leds.show(); //not sure if I need to call this or not...
  }
  cmdstate++;
}


