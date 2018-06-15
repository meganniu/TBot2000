#include <AFMotor.h>

AF_Stepper teabag(200, 2);

int pump = 50;
int kettle = 28;

// Kettle debounce variables
int kettle_on = 0;             // the current reading from the input pin
int kettle_last_state = LOW;   // the previous reading from the input pin

// the following variables are unsigned long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long last_debounce_time = 0;  // the last time the output pin was toggled
const long DEBOUNCE_DELAY = 1000;    // the debounce time; increase if the output flickers

int digits[] = {33, 32, 31, 30};
int timer_pot = A15;
int steep_time = 4;

int boil_indicator = 36;
int steep_indicator = 37;
int ready_indicator = 38;

void show_digit(int n) {
  if (n < 0) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(digits[i], LOW);
    }
  }

  int bits[] = {
    (n & B1000) >> 3,
    (n & B0100) >> 2,
    (n & B0010) >> 1,
    n & B0001
  };

  for (int i = 0; i < 4; i++) {
    digitalWrite(digits[i], bits[i]);
  }
}

void setup() {
  // Set up motors
  pinMode(pump, OUTPUT);
  digitalWrite(pump, LOW);

  teabag.setSpeed(60);

  pinMode(kettle, INPUT_PULLUP);

  for (int i = 0; i < 4; i++) {
    pinMode(digits[i], OUTPUT);
    digitalWrite(digits[i], LOW);
  }
  show_digit(steep_time);

  pinMode(timer_pot, INPUT);

  // Indicator LEDs
  pinMode(boil_indicator, OUTPUT);
  pinMode(steep_indicator, OUTPUT);
  pinMode(ready_indicator, OUTPUT);
  digitalWrite(boil_indicator, LOW);
  digitalWrite(steep_indicator, LOW);
  digitalWrite(ready_indicator, LOW);
}

void loop() {
  while (!kettle_on) { // While kettle is off
    // Read pot to determine steep time
    int pot_reading = analogRead(timer_pot);
    steep_time = constrain(round(pot_reading/(3.2*256.0) * 9), 0, 9); // 3.2: magic scaling factor

    show_digit(steep_time);

    // Check kettle with debounce
    int reading = digitalRead(kettle); // read the state of the switch into a local variable

    if (reading != kettle_last_state) { // If the switch changed, due to noise or pressing
      last_debounce_time = millis();  // reset the debouncing timer
    }

    if ((millis() - last_debounce_time) > DEBOUNCE_DELAY) { // essentially one press
      if (reading != kettle_on) {  // if the button state has changed
        kettle_on = reading;
      }
    }

    kettle_last_state = reading; // save the reading.  Next time through the loop, it'll be the lastButtonState
  }
  digitalWrite(ready_indicator, LOW);

  // Water starts boiling
  digitalWrite(boil_indicator, HIGH);
  while (kettle_on) { // Wait for boiling to finish
    // Check kettle with debounce
    int reading = digitalRead(kettle); // read the state of the switch into a local variable

    if (reading != kettle_last_state) { // If the switch changed, due to noise or pressing
      last_debounce_time = millis();  // reset the debouncing timer
    }

    if ((millis() - last_debounce_time) > DEBOUNCE_DELAY) { // essentially one press
      if (reading != kettle_on) {  // if the button state has changed
        kettle_on = reading;
      }
    }

    kettle_last_state = reading; // save the reading.  Next time through the loop, it'll be the lastButtonState
  }

  // Pump the water to the mug
  digitalWrite(pump, HIGH);
  // Flash boil LED while pumping
  for (int i = 0; i < 285; i++) { // 4:45 to transport one mugful of water, 1s per loop
    digitalWrite(boil_indicator, HIGH);
    delay(500);
    digitalWrite(boil_indicator, LOW);
    delay(500);
  }
  digitalWrite(pump, LOW);

  teabag.step(1200, BACKWARD, SINGLE); // Lower teabag

  // Wait for tea to steep
  digitalWrite(steep_indicator, HIGH);
  for (int i = steep_time; i >= 0; i--) {
    delay(60000); // Wait one minute
    show_digit(i);
  }
  digitalWrite(steep_indicator, LOW);

  teabag.step(1200, FORWARD, SINGLE); // Raise teabag

  // Tea is ready
  digitalWrite(ready_indicator, HIGH);
}
