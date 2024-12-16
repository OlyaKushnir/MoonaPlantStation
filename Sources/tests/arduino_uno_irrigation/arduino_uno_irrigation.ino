#define PIN_VALVE_SUPPLY D3   // Water supply valve
#define PIN_VALVE_IRRIGATION D7 // Irrigation valve
#define PIN_DRAIN_PUMP D8      // Drain pump

#define WATER_LEVEL_SENSOR A0  // Water tank level sensor
#define DRAIN_SENSOR A1        // Drain water level sensor

// Timer variables
unsigned long irrigationStartTime = 0; // Start time for irrigation
bool irrigationActive = false; // State of irrigation
bool drainActive = false; // State of drain pump

int waterLevelThreshold = 500; // Threshold for water level sensor (adjust as needed)
int drainThreshold = 700;      // Threshold for drain sensor (adjust as needed)

void setup() {
    pinMode(PIN_VALVE_SUPPLY, OUTPUT);
    pinMode(PIN_VALVE_IRRIGATION, OUTPUT);
    pinMode(PIN_DRAIN_PUMP, OUTPUT);

    digitalWrite(PIN_VALVE_SUPPLY, LOW);
    digitalWrite(PIN_VALVE_IRRIGATION, LOW);
    digitalWrite(PIN_DRAIN_PUMP, LOW);

    Serial.begin(9600);
}

void startIrrigation() {
    digitalWrite(PIN_VALVE_SUPPLY, HIGH);
    delay(500);
    digitalWrite(PIN_VALVE_IRRIGATION, HIGH);
    irrigationStartTime = millis();
    irrigationActive = true;
    Serial.println("Irrigation started");
}

void stopIrrigation() {
    digitalWrite(PIN_VALVE_IRRIGATION, LOW);
    delay(500);
    digitalWrite(PIN_VALVE_SUPPLY, LOW);
    irrigationActive = false;
    Serial.println("Irrigation stopped");
}

void startDrain() {
    digitalWrite(PIN_DRAIN_PUMP, HIGH);
    drainActive = true;
    Serial.println("Drain pump started");
}

void stopDrain() {
    digitalWrite(PIN_DRAIN_PUMP, LOW);
    drainActive = false;
    Serial.println("Drain pump stopped");
}

void loop() {
    int waterLevel = analogRead(WATER_LEVEL_SENSOR);
    int drainLevel = analogRead(DRAIN_SENSOR);

    Serial.print("Water Level: ");
    Serial.println(waterLevel);

    Serial.print("Drain Level: ");
    Serial.println(drainLevel);

    // Irrigation logic
    if (irrigationActive && waterLevel >= waterLevelThreshold) {
        stopIrrigation();
    }

    if (!irrigationActive && waterLevel < waterLevelThreshold) {
        startIrrigation();
    }

    // Drain pump logic
    if (!drainActive && drainLevel >= drainThreshold) {
        startDrain();
    }

    if (drainActive && drainLevel < drainThreshold) {
        stopDrain();
    }

    delay(500); // Short delay for stability
}
