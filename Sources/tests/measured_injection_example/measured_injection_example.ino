// Define the pin numbers
#define WATER_LEVEL_PIN A0

// pins for pulse engine
#define DIR_PIN 5    // Direction Pin
#define STEP_PIN 3   // Step Pin
#define ENABLE_PIN 4 // Enable Pin (optional)

bool dir = false;
int speed = 50;
int pulses = 3333;

void setup() {
  Serial.begin(9600);

    // Set pins as outputs
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  
  // Optionally enable the driver
  digitalWrite(ENABLE_PIN, LOW);  // Enable driver (LOW or HIGH based on your driver)

  // Set initial direction (HIGH or LOW)
  digitalWrite(DIR_PIN, LOW);  // Rotate clockwise

}

void loop() {
  static bool continousPumpEnabled = false;

  int command = Serial.read();
  if (command == -1) // return if no command or new line
  {
    if (continousPumpEnabled)
      movePump(pulses, speed);
  }
  if (command != -1 && command != 10)
    Serial.println("Recieved command: " + String(command));
  
  switch (command) {
    case 97:  // command = 'a' - single pulse series
      movePump(pulses, speed); 
      break;
    case 115: // command = 's' - start/stop continous
      continousPumpEnabled = !continousPumpEnabled;
      break;
    case 100: // command = 'd' - flip direction
      dir = !dir;
      (dir ? digitalWrite(DIR_PIN, HIGH) : digitalWrite(DIR_PIN, LOW));
      break;
    case 119: // command = 'w' - ger water level
      int waterLevelValue = analogRead(WATER_LEVEL_PIN);
      Serial.println("Water Level:" + String(waterLevelValue));
      break;
    default: 
      break;
  }

}



void movePump(int pulseCount, int pulseDelay) {
  // Generate steps
  for (int i = 0; i < pulseCount; i++) {
    digitalWrite(STEP_PIN, HIGH);   // Send a pulse
    delayMicroseconds(pulseDelay);        // Adjust speed
    digitalWrite(STEP_PIN, LOW);    // End pulse
    delayMicroseconds(pulseDelay);        // Adjust speed
  }
}

