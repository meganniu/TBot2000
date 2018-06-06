int pump = 52;
int kettle = 40;

int digits[] = {33, 32, 31, 30};

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
  pinMode(pump, OUTPUT);
  pinMode(kettle, INPUT);

  for (int i = 0; i < 4; i++) {
    pinMode(digits[i], OUTPUT);
    digitalWrite(digits[i], LOW);
  }
}

void loop() {
  digitalWrite(pump, digitalRead(kettle));
}

