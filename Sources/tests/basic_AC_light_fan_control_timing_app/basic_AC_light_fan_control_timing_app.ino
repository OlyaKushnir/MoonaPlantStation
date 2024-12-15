// CONNECTIONS:
// DS1302 CLK/SCLK --> 5
// DS1302 DAT/IO --> 4
// DS1302 RST/CE --> 2
// DS1302 VCC --> 3.3v - 5v
// DS1302 GND --> GND

#include <RtcDS1302.h>

#include <Wire.h>
#define I2C_ADDRESS 0x08 // Unique address for this Arduino as an I2C slave

ThreeWire myWire(4, 5, 2); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

// Define the pin numbers
#define PIN_LIGHT_RELAY 10
#define PIN_FAN_RELAY 11

// Variables for light
bool currLightState = false;

// Variables for fan
unsigned long lastFanToggle = 0; // Time of the last fan toggle
bool fanState = false;           // Current state of the fan (ON/OFF)

// Light control
void setupLight() {
  pinMode(PIN_LIGHT_RELAY, OUTPUT);
}
void setLight_ON() {
  digitalWrite(PIN_LIGHT_RELAY, HIGH);
}
void setLight_OFF() {
  digitalWrite(PIN_LIGHT_RELAY, LOW);
}

// Fan control
void setupFan() {
  pinMode(PIN_FAN_RELAY, OUTPUT);
}
void setFan_ON() {
  digitalWrite(PIN_FAN_RELAY, HIGH);
}
void setFan_OFF() {
  digitalWrite(PIN_FAN_RELAY, LOW);
}

void setup() {
  Wire.begin(I2C_ADDRESS); // Initialize in slave mode
  Wire.onRequest(sendData); // Set handler for requests from the master
  //Wire.onReceive(receiveData); // Set handler for receiving data from the master

  setupLight();
  setupFan();

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
    // Check if 15 minutes have passed since the last toggle
    if (millis() - lastFanToggle >= 15 * 60 * 1000) { // 15 minutes
      lastFanToggle = millis(); // Update the last toggle time
      fanState = !fanState;     // Toggle the fan state

      if (fanState) {
        setFan_ON();
        Serial.println("Fan turned ON (night cycle)");
      } else {
        setFan_OFF();
        Serial.println("Fan turned OFF (night cycle)");
      }
    }
  }  else { // Daytime mode (8:00–18:00)
    if (!fanState) {
      setFan_ON(); // Ensure the fan is ON during the day
      fanState = true;
      Serial.println("Fan turned ON (daytime)");
    }
  }
  
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


// Function to send the light and fan states to the master
void sendData() {
  // Convert boolean states to bytes (1 for ON, 0 for OFF)
  uint8_t lightStateByte = currLightState ? 1 : 0;
  uint8_t fanStateByte = fanState ? 1 : 0;

  // Send light and fan states over I2C
  Wire.write(lightStateByte); // Send light state
  Wire.write(fanStateByte);   // Send fan state
}