// Define the pin numbers
#define VALVE_1_PIN 10
#define SPLIT_VALVE_1_PIN 11
#define VALVE_2_PIN 12
#define SPLIT_VALVE_2_PIN 13
#define FEEDER_VALVE_PIN 14


// Define the delay time in milliseconds
#define DELAY_TIME 1000  // 1 second


void initDigitalOutputPin(int pin) {
  pinMode(pin, OUTPUT);
}
void setDigitalOutputPin(int pin) {
  digitalWrite(pin, HIGH);
}
void resetDigitalOutputPin(int pin) {
  digitalWrite(pin, LOW);
}
void testPin(int pin) {
  resetDigitalOutputPin(pin);
  delay(DELAY_TIME);
  setDigitalOutputPin(pin);
}

void setup() {
  // Initialize the pins as OUTPUT
  initDigitalOutputPin(VALVE_1_PIN);
  initDigitalOutputPin(SPLIT_VALVE_1_PIN);
  initDigitalOutputPin(VALVE_2_PIN);
  initDigitalOutputPin(SPLIT_VALVE_2_PIN);
  initDigitalOutputPin(FEEDER_VALVE_PIN);
}


void loop() {
  testPin(VALVE_1_PIN);
  testPin(SPLIT_VALVE_1_PIN);
  testPin(VALVE_2_PIN);
  testPin(SPLIT_VALVE_2_PIN);
  testPin(FEEDER_VALVE_PIN);
}
