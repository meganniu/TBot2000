#include <AFMotor.h>

AF_Stepper teabag(200, 1);

int pump = 50;
int kettle = 40;

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
  Serial.begin(9600);

  // Set up motors
  pinMode(pump, OUTPUT);
  digitalWrite(pump, LOW);

  teabag.setSpeed(60);

  pinMode(kettle, INPUT);

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
  digitalWrite(pump, digitalRead(kettle));

  // Read pot to determine steep time
  int reading = analogRead(timer_pot);
  steep_time = constrain(round(reading/(3.2*256.0) * 9), 0, 9); // 3.2: magic scaling factor

  show_digit(steep_time);
}
