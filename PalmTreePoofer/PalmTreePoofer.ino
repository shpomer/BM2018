
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

#define LOG_INTERVAL 1000

elapsedMillis timer;
int loggedTimer;
Flamethrower leftFire = Flamethrower(SOLENOID_LEFT, LED_LEFT, 15);
Flamethrower rightFire = Flamethrower(SOLENOID_RIGHT, LED_RIGHT, 80);

Flamethrower *fire1 = &leftFire;
Flamethrower *fire2 = &rightFire;
/*
const uint32_t     techno_beat_times[]     = {     0,   126,   252,   378,   504,   630,   756,  1405,  1511,  1952,  2102,  2253,  2393,  2833,  3053,  3273 };
const int          techno_beat_durations[] = {    -1,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0 };
Flamethrower *techno_beat_flamethrowers[] =  { fire1, fire1, fire1, fire1, fire1, fire2, fire1, fire1, fire2, fire1, fire1, fire1, fire2, fire1, fire1, fire2 };
const unsigned int techno_beat_length = sizeof(techno_beat_times)/sizeof(uint32_t);
const uint32_t     techno_beat_duration = 3525;
*/

#define F1_S 17
#define F1_B 18
#define F1_N 14
#define F2_S 60
#define F2_B 80
#define F2_N 55
#define BEAT 300

const uint32_t     techno_beat_times[]     = {     0,   BEAT, 1.5*BEAT, 2*BEAT, 3*BEAT, 3*BEAT, 5*BEAT, 5*BEAT, 5.5*BEAT, 6*BEAT, 7*BEAT,  2102,  2253,  2393,  2833,  3053,  3273 };
const int          techno_beat_durations[] = {  F2_S,   F1_N,     F2_S,   F2_B,   F1_N,   F2_S,   F1_N,   F2_S,     F2_S,   F2_B,   F1_N,     0,     0,     0,     0,     0,     0 };
Flamethrower *techno_beat_flamethrowers[] =  { fire2,  fire1,    fire2,  fire2,  fire1,  fire2,  fire1,  fire2,    fire2,  fire2,  fire1, fire1, fire1, fire2, fire1, fire1, fire2 };
const unsigned int techno_beat_length = 11;
const uint32_t     techno_beat_duration = 8*BEAT;


Beat technoBeat = Beat(techno_beat_times, techno_beat_durations, techno_beat_flamethrowers, techno_beat_length, techno_beat_duration);


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
    bool bottom_left_released = debouncer_bottom_left.risingEdge();
    bool bottom_right_pressed = debouncer_bottom_right.fallingEdge();
    bool bottom_right_released = debouncer_bottom_right.risingEdge();

    if(timer % LOG_INTERVAL == 0)
    {
        if(timer / LOG_INTERVAL != loggedTimer)
        {
            loggedTimer = timer / LOG_INTERVAL;
            Serial.print("time: ");
            Serial.println(((float)timer)/1000);
        }
    }

    leftFire.process(timer);
    rightFire.process(timer);

    technoBeat.process(timer);

    // Turn on or off the LED as determined by the state :
    if ( top_left_pressed )
    {
        Serial.println("Poof!");

        //leftFire.poof(timer, 15);
        rightFire.poof(timer, 54);
    }

    if( top_middle_pressed )
    {
        technoBeat.start(timer);
    }

    if( top_right_pressed )
    {
        technoBeat.stop();
    }

    if( bottom_right_pressed )
    {
        rightFire.stop();
        rightFire.poof(timer, MAX_POOF_DURATION);
    }
    if( bottom_right_released )
    {
        rightFire.stop();
    }

    if( bottom_left_pressed )
    {
        leftFire.stop();
        leftFire.poof(timer, MAX_POOF_DURATION);
    }
    if( bottom_left_released )
    {
        leftFire.stop();
    }
}


