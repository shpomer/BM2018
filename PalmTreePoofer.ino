
/* 
 DESCRIPTION
 ====================
 Simple example of the Bounce library that switches the debug LED when a button is pressed.
 */
// Include the Bounce2 library found here :
// https://github.com/thomasfredericks/Bounce-Arduino-Wiring
#include <Bounce2.h>

#define BUTTON_TOP_LEFT_PIN 1
#define BUTTON_TOP_MIDDLE_PIN 3
#define BUTTON_TOP_RIGHT_PIN 4
#define BUTTON_BOTTOM_LEFT_PIN 2
#define BUTTON_BOTTOM_RIGHT_PIN 20

#define SOLENOID_LEFT 13
#define SOLENOID_RIGHT 18
#define LED_LEFT 19
#define LED_RIGHT 12

#define MAX_POOF_DURATION 60

elapsedMillis timer;

int leftPoofEndTime = 0;
int rightPoofEndTime = 0;

bool leftPoofing = 0;
bool rightPoofing = 0;

// Instantiate a Bounce object
Bounce debouncer_top_left = Bounce();
Bounce debouncer_top_middle = Bounce();
Bounce debouncer_top_right = Bounce();
Bounce debouncer_bottom_left = Bounce();
Bounce debouncer_bottom_right = Bounce();

void left_fire_on(int duration = 0)
{
    if(duration < 1 || duration > MAX_POOF_DURATION)
    {
        duration = MAX_POOF_DURATION;
    }
    leftPoofEndTime = timer + duration;
    
    digitalWrite(SOLENOID_LEFT, HIGH);
    digitalWrite(LED_LEFT, HIGH);
    
    leftPoofing = true;
}

void left_fire_off()
{
    digitalWrite(SOLENOID_LEFT, LOW);
    digitalWrite(LED_LEFT, LOW);
    
    leftPoofing = false;
}

void right_fire_on(int duration = 0)
{
    if(duration < 1 || duration > MAX_POOF_DURATION)
    {
        duration = MAX_POOF_DURATION;
    }
    rightPoofEndTime = timer + duration;
    
    digitalWrite(SOLENOID_RIGHT, HIGH);
    digitalWrite(LED_RIGHT, HIGH);
    
    rightPoofing = true;
}

void right_fire_off()
{
    digitalWrite(SOLENOID_RIGHT, LOW);
    digitalWrite(LED_RIGHT, LOW);
    
    rightPoofing = false;
}

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

    left_fire_off();
    right_fire_off();
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

    if ( leftPoofing && timer > leftPoofStart + POOF_DURATION )
    {
        left_fire_off();
    }

    // Turn on or off the LED as determined by the state :
    if ( top_left_pressed && !leftPoofing && !rightPoofing ) {
        Serial.println("button 0 pressed");
        if(!leftPoofing)
        {
            digitalWrite(SOLENOID_LEFT, HIGH);
            digitalWrite(SOLENOID_RIGHT, HIGH);
            digitalWrite(LED_RIGHT, HIGH);
            digitalWrite(LED_LEFT, HIGH);
            leftPoofStart = timer;
            leftPoofing = 1;
        }
  } 
  
}


