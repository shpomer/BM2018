
/* 
 DESCRIPTION
 ====================
 Simple example of the Bounce library that switches the debug LED when a button is pressed.
 */
// Include the Bounce2 library found here :
// https://github.com/thomasfredericks/Bounce-Arduino-Wiring
#include <Bounce2.h>
#include <Arduino.h>
#include "flamethrower.h"
#include "beat.h"

#define BUTTON_TOP_LEFT_PIN 1
#define BUTTON_TOP_MIDDLE_PIN 3
#define BUTTON_TOP_RIGHT_PIN 4
#define BUTTON_BOTTOM_LEFT_PIN 2
#define BUTTON_BOTTOM_RIGHT_PIN 20

#define SOLENOID_LEFT 13
#define SOLENOID_RIGHT 18
#define LED_LEFT 19
#define LED_RIGHT 12

elapsedMillis timer;
int loggedTimer;
Flamethrower leftFire = Flamethrower(SOLENOID_LEFT, LED_LEFT);
Flamethrower rightFire = Flamethrower(SOLENOID_RIGHT, LED_RIGHT);

const unsigned int techno_beat_intervals[] = {        100,        100,        100,        100,        500 };
const unsigned int techno_beat_durations[] = {         20,         20,         20,         20,         60 };
Flamethrower *techno_beat_flamethrowers[] =  {  &leftFire,  &leftFire,  &leftFire,  &leftFire, &rightFire };
const unsigned int techno_beat_length = 5;
Beat technoBeat = Beat(techno_beat_intervals, techno_beat_durations, techno_beat_flamethrowers, techno_beat_length);

// Instantiate a Bounce object
Bounce debouncer_top_left = Bounce();
Bounce debouncer_top_middle = Bounce();
Bounce debouncer_top_right = Bounce();
Bounce debouncer_bottom_left = Bounce();
Bounce debouncer_bottom_right = Bounce();

void setup()
{
    // Setup the button with an internal pull-up :
    pinMode(BUTTON_TOP_LEFT_PIN,INPUT_PULLUP);
    pinMode(BUTTON_TOP_MIDDLE_PIN,INPUT_PULLUP);
    pinMode(BUTTON_TOP_RIGHT_PIN,INPUT_PULLUP);
    pinMode(BUTTON_BOTTOM_LEFT_PIN,INPUT_PULLUP);
    pinMode(BUTTON_BOTTOM_RIGHT_PIN,INPUT_PULLUP);

    // After setting up the button, setup the Bounce instance :
    debouncer_top_left.attach(BUTTON_TOP_LEFT_PIN);
    debouncer_top_middle.attach(BUTTON_TOP_MIDDLE_PIN);
    debouncer_top_right.attach(BUTTON_TOP_RIGHT_PIN);
    debouncer_bottom_left.attach(BUTTON_BOTTOM_LEFT_PIN);
    debouncer_bottom_right.attach(BUTTON_BOTTOM_RIGHT_PIN);
  
    debouncer_top_left.interval(5); // interval in ms
    debouncer_top_middle.interval(5); // interval in ms
    debouncer_top_right.interval(5); // interval in ms
    debouncer_bottom_left.interval(5); // interval in ms
    debouncer_bottom_right.interval(5); // interval in ms
}

void loop()
{
    // Update the Bounce instance :
    debouncer_top_left.update();
    debouncer_top_middle.update();
    debouncer_top_right.update();
    debouncer_bottom_left.update();
    debouncer_bottom_right.update();

    // Get the updated value :
    bool top_left_pressed = debouncer_top_left.fallingEdge();
    bool top_middle_pressed = debouncer_top_middle.fallingEdge();
    bool top_right_pressed = debouncer_top_right.fallingEdge();
    bool bottom_left_pressed = debouncer_bottom_left.fallingEdge();
    bool bottom_right_pressed = debouncer_bottom_right.fallingEdge();

    if(timer % 1000 == 0)
    {
        if(timer != loggedTimer)
        {
            loggedTimer = timer;
        }
    }

    leftFire.process(timer);
    rightFire.process(timer);

    technoBeat.process(timer);

    // Turn on or off the LED as determined by the state :
    if ( top_left_pressed )
    {
        Serial.println("Poof!");

        leftFire.poof(timer, 14);
        rightFire.poof(timer, 14);
    }

    if( top_middle_pressed )
    {
        technoBeat.start(timer);
    }

    if( top_right_pressed)
    {
        technoBeat.stop();
    }
}


