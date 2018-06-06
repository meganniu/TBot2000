#include <AFMotor.h>

AF_Stepper teabag(200, 1);

int pump = 52;
int kettle = 40;

void setup() {
  Serial.begin(9600);

  // Set up motors
  pinMode(pump, OUTPUT);
  digitalWrite(pump, LOW);

  teabag.setSpeed(5000);

  pinMode(kettle, INPUT_PULLUP);
}

void loop() {
  Serial.println("Kettle switch closed (stopped)");
  while (digitalRead(kettle) == HIGH);
  Serial.println("Kettle switch open (started)");
  digitalWrite(pump, HIGH);
  delay(1000);
  while (digitalRead(kettle) == LOW);
}
