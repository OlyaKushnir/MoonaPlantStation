// Define pins for the A4988
#define DIR_PIN 5    // Direction Pin
#define STEP_PIN 3   // Step Pin
#define ENABLE_PIN 4 // Enable Pin (optional)

bool dir = false;
int delayMS = 50;

void setup() {
  // Set pins as outputs
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  
  // Optionally enable the driver
  digitalWrite(ENABLE_PIN, LOW);  // Enable driver (LOW or HIGH based on your driver)

  // Set initial direction (HIGH or LOW)
  //digitalWrite(DIR_PIN, HIGH);  // Rotate clockwise
}

void loop() {
  (dir ? digitalWrite(DIR_PIN, HIGH) : digitalWrite(DIR_PIN, LOW));
  //dir = !dir;

  delayMicroseconds(1000);

  // Generate steps
  for (int i = 0; i < 7500; i++) {  // Move 200 steps (full revolution for most NEMA 17 motors)
    digitalWrite(STEP_PIN, HIGH);   // Send a pulse
    delayMicroseconds(delayMS);        // Adjust speed
    digitalWrite(STEP_PIN, LOW);    // End pulse
    delayMicroseconds(delayMS);        // Adjust speed
  }
  
  //delay(1000);  // Wait 1 second before reversing the direction
}
