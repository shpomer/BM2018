
/* 
 DESCRIPTION
 ====================
 Simple example of the Bounce library that switches the debug LED when a button is pressed.
 */
// Include the Bounce2 library found here :
// https://github.com/thomasfredericks/Bounce-Arduino-Wiring
#include <Bounce2.h>

#define BUTTON_PIN1 1
#define BUTTON_PIN3 3
#define BUTTON_PIN4 4
#define BUTTON_PIN_LEFT 2
#define BUTTON_PIN_RIGHT 20

#define SOLENOID_LEFT 13
#define SOLENOID_RIGHT 18
#define LED_LEFT 19
#define LED_RIGHT 12

// Instantiate a Bounce object
Bounce debouncer0 = Bounce(); 
Bounce debouncer1 = Bounce(); 
Bounce debouncer2 = Bounce(); 
Bounce debouncer3 = Bounce(); 
Bounce debouncer4 = Bounce(); 

void setup() {

  // Setup the button with an internal pull-up :
  pinMode(BUTTON_PIN1,INPUT_PULLUP);
  pinMode(BUTTON_PIN3,INPUT_PULLUP);
  pinMode(BUTTON_PIN4,INPUT_PULLUP);
  pinMode(BUTTON_PIN_LEFT,INPUT_PULLUP);
  pinMode(BUTTON_PIN_RIGHT,INPUT_PULLUP);

  pinMode(SOLENOID_RIGHT, OUTPUT);
  pinMode(SOLENOID_LEFT, OUTPUT);

  // After setting up the button, setup the Bounce instance :
  debouncer0.attach(BUTTON_PIN1);
  debouncer1.attach(BUTTON_PIN3);
  debouncer2.attach(BUTTON_PIN4);
  debouncer3.attach(BUTTON_PIN_LEFT);
  debouncer4.attach(BUTTON_PIN_RIGHT);
  debouncer0.interval(5); // interval in ms
  debouncer1.interval(5); // interval in ms
  debouncer2.interval(5); // interval in ms
  debouncer3.interval(5); // interval in ms
  debouncer4.interval(5); // interval in ms

  
}

void loop() {
  // Update the Bounce instance :
  debouncer0.update();
  debouncer1.update();
  debouncer2.update();
  debouncer3.update();
  debouncer4.update();
  
  // Get the updated value :
  int value0 = debouncer0.read();
  int value1 = debouncer1.read();
  int value2 = debouncer2.read();
  int value3 = debouncer3.read();
  int value4 = debouncer4.read();

  // Turn on or off the LED as determined by the state :
  if ( value0 == LOW ) {
    Serial.println("button 0 pressed");
  } 
  else {
  }
  // Turn on or off the LED as determined by the state :
  if ( value0 == LOW ) {
    Serial.println("button 1 pressed");
    digitalWrite(SOLENOID_LEFT, HIGH);
  } 
  else {
    digitalWrite(SOLENOID_LEFT, LOW);
  }
  // Turn on or off the LED as determined by the state :
  if ( value1 == LOW ) {
    Serial.println("button 2 pressed");
    digitalWrite(SOLENOID_RIGHT, HIGH);
  } 
  else {
    digitalWrite(SOLENOID_RIGHT, LOW);
  }
  // Turn on or off the LED as determined by the state :
  if ( value2 == LOW ) {
    Serial.println("button 3 pressed");
  } 
  else {
  }
  // Turn on or off the LED as determined by the state :
  if ( value3 == LOW ) {
    Serial.println("button LEFT pressed");
    digitalWrite(SOLENOID_LEFT, HIGH);
    digitalWrite(LED_LEFT, HIGH);
  } 
  else {
    digitalWrite(SOLENOID_LEFT, LOW);
    digitalWrite(LED_LEFT, LOW);
  }
  // Turn on or off the LED as determined by the state :
  if ( value4 == LOW ) {
    Serial.println("button RIGHT pressed");
    digitalWrite(SOLENOID_RIGHT, HIGH);
    digitalWrite(LED_RIGHT, HIGH);
  } 
  else {
    digitalWrite(SOLENOID_RIGHT, LOW);
    digitalWrite(LED_RIGHT, LOW);
  }
  
}


