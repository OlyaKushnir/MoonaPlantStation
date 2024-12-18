// CONNECTIONS:
// DS1302 CLK/SCLK --> 5
// DS1302 DAT/IO --> 4
// DS1302 RST/CE --> 2
// DS1302 VCC --> 3.3v - 5v
// DS1302 GND --> GND

#include <RtcDS1302.h>
#include <Wire.h>

#define I2C_ADDRESS 0x08 // Unique address for this Arduino as an I2C slave

// Sensor and pump pins
#define SENSOR_PIN A0  // Pin to read sensor data
#define PUMP_PIN 9     // Pin to control the Pump

int waterLevel = 0; // Variable to store water level reading

// Pump control variables
unsigned long pumpStartTime = 0;    // Stores the time when pump was turned ON
const unsigned long pumpRunLimit = 60 * 1000; // 1-minute runtime limit
bool pumpRunning = false;           // State of the pump
const int waterThreshold = 300;     // Water level threshold
const int debounceDelay = 3000;     // 3 seconds debounce time

// DS1302 setup
ThreeWire myWire(4, 5, 2); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

// Relay pins
#define PIN_LIGHT_RELAY 10
#define PIN_FAN_RELAY 11

// Variables for light
bool currLightState = false;

// Variables for fan
unsigned long lastFanToggle = 0; // Time of the last fan toggle
bool fanState = false;           // Current state of the fan (ON/OFF)

// Light control functions
void setupLight() {
  pinMode(PIN_LIGHT_RELAY, OUTPUT);
}
void setLight_ON() {
  digitalWrite(PIN_LIGHT_RELAY, HIGH);
}
void setLight_OFF() {
  digitalWrite(PIN_LIGHT_RELAY, LOW);
}

// Fan control functions
void setupFan() {
  pinMode(PIN_FAN_RELAY, OUTPUT);
}
void setFan_ON() {
  digitalWrite(PIN_FAN_RELAY, HIGH);
}
void setFan_OFF() {
  digitalWrite(PIN_FAN_RELAY, LOW);
}

// Water level sensor reading function
int readWaterLevel() {
  waterLevel = analogRead(SENSOR_PIN);  // Read the analog value from the water sensor
  return waterLevel;                    // Return the water level reading
}

// Function to check stable water level
bool isWaterLevelStable() {
  static unsigned long stableStartTime = 0;

  if (waterLevel >= waterThreshold) {
    if (stableStartTime == 0) {
      stableStartTime = millis(); // Start timing if water is detected
    } else if (millis() - stableStartTime >= debounceDelay) {
      return true; // Water level is stable
    }
  } else {
    stableStartTime = 0; // Reset timing if water level drops
  }
  return false;
}

// Pump control function with timeout and sensor check
void controlPumpWithTimeout() {
  if (!pumpRunning && isWaterLevelStable()) {
    // Start the pump
    digitalWrite(PUMP_PIN, HIGH);
    pumpStartTime = millis();
    pumpRunning = true;
    Serial.println("Pump started!");
  }

  // If pump is running, check for timeout or low water level
  if (pumpRunning) {
    if (millis() - pumpStartTime >= pumpRunLimit || waterLevel < waterThreshold) {
      digitalWrite(PUMP_PIN, LOW);
      pumpRunning = false;
      Serial.println("Pump stopped (timeout or low water level).");
    }
  }
}

void setup() {
  Wire.begin(I2C_ADDRESS); // Initialize in slave mode
  Wire.onRequest(sendData); // Set handler for requests from the master
  
  setupLight();
  setupFan();
  pinMode(PUMP_PIN, OUTPUT); // Initialize Pump pin as an output

  Serial.begin(9600);
  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.println(__TIME__);
  
  Rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  printDateTime(compiled);
  Serial.println();
  
  if (!Rtc.IsDateTimeValid()) {
    Serial.println("RTC lost confidence in the DateTime!");
    Rtc.SetDateTime(compiled);
  }
  
  if (Rtc.GetIsWriteProtected()) {
    Serial.println("RTC was write protected, enabling writing now");
    Rtc.SetIsWriteProtected(false);
  }
  
  if (!Rtc.GetIsRunning()) {
    Serial.println("RTC was not actively running, starting now");
    Rtc.SetIsRunning(true);
  }
  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) {
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
    Rtc.SetDateTime(compiled);
  } else if (now > compiled) {
    Serial.println("RTC is newer than compile time. (this is expected)");
  } else if (now == compiled) {
    Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }
}

void loop() {
  RtcDateTime now = Rtc.GetDateTime();
  printDateTime(now);
  Serial.println();

  if (!now.IsValid()) {
    Serial.println("RTC lost confidence in the DateTime!");
  }

  // Light control logic
  if (now.Hour() >= 0 && now.Hour() <= 6) { // OFF
    setLight_OFF();
    if (currLightState == true) {
      Serial.println("Light switched OFF");
      currLightState = false;
    }
  } else { // ON
    setLight_ON();
    if (currLightState == false) {
      Serial.println("Light switched ON");
      currLightState = true;
    }
  }

  // Fan control logic
  if (now.Hour() >= 18 || now.Hour() < 8) { // Night mode
    if (millis() - lastFanToggle >= 15 * 60 * 1000) { // 15 minutes
      lastFanToggle = millis();
      fanState = !fanState;

      if (fanState) {
        setFan_ON();
        Serial.println("Fan turned ON (night cycle)");
      } else {
        setFan_OFF();
        Serial.println("Fan turned OFF (night cycle)");
      }
    }
  } else { // Daytime mode
    if (!fanState) {
      setFan_ON();
      fanState = true;
      Serial.println("Fan turned ON (daytime)");
    }
  }
  
  // Read the water level sensor and print the value
  int level = readWaterLevel();
  Serial.print("Water level: ");
  Serial.println(level);

  // Control the pump with timeout and stability check
  controlPumpWithTimeout();

  delay(1000); // Wait for 1 second before the next iteration
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt) {
  char datestring[26];
  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
             dt.Month(),
             dt.Day(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second());
  Serial.print(datestring);
}

// Function to send the light, fan, and water states to the master
void sendData() {
  uint8_t lightStateByte = currLightState ? 1 : 0;
  uint8_t fanStateByte = fanState ? 1 : 0;
  uint8_t waterLevelByte = map(waterLevel, 0, 1023, 0, 255); 

  Wire.write(lightStateByte);
  Wire.write(fanStateByte);
  Wire.write(waterLevelByte);
}
