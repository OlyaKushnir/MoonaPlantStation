
int waterLevelPins[] = {A0, A1, A2, A3};
int numSensors = sizeof(waterLevelPins) / sizeof(waterLevelPins[0]);

void setup() {
  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i < numSensors; i++) {
    int waterLevelValue = analogRead(waterLevelPins[i]);
    Serial.print("Sensor"+String(i+1)+":"+String(waterLevelValue));
    if (i != numSensors-1) Serial.print(",");
  }
  Serial.println();
  delay(500);

/*
  // Print the sensor values to the Serial Plotter
  Serial.print("Sensor1:");
  Serial.print(waterLevelPins[0]);
  Serial.print(",");
  Serial.print("Sensor2:");
  Serial.print(waterLevelPins[1]);
  Serial.print(",");
  Serial.print("Sensor3:");
  Serial.print(waterLevelPins[2]);
  Serial.print(",");
  Serial.print("Sensor4:");
  Serial.println(waterLevelPins[3]);
  delay(500);
  */
}
