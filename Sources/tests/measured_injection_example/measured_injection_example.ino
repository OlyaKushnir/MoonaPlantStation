// Define the pin numbers
#define WATER_LEVEL_PIN A0

void setup() {
  Serial.begin(9600);
}

void loop() {
  int waterLevelValue = analogRead(WATER_LEVEL_PIN);
  Serial.println("Water Level:" + String(waterLevelValue));
  delay(200);
}
