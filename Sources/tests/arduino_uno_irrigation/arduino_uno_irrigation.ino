// CONNECTIONS:
// DS1302 CLK/SCLK --> 5
// DS1302 DAT/IO --> 4
// DS1302 RST/CE --> 2
// DS1302 VCC --> 3.3v - 5v
// DS1302 GND --> GND

#include <RtcDS1302.h>
#include <Wire.h>

#define I2C_ADDRESS 0x08          // I2C address of the fertilizer Arduino
#define PIN_VALVE_SUPPLY 10        // Pin for water supply valve 6
#define PIN_VALVE_IRRIGATION 11    // Pin for irrigation valve 7
#define WATER_LEVEL_SENSOR A0     // Analog pin for water level sensor А1
#define waterLevelThreshold 300   // Target water level threshold


// DS1302 setup
ThreeWire myWire(4, 5, 2); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

bool supplyActive = true;        // Trigger for water supply
bool irrigationActive = false;    // Trigger for irrigation
bool fertilizationDone = true; //false;   // Flag to track fertilization step

unsigned long irrigationStartTime = 0;

void setup() {
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
    Wire.begin();                    // Initialize I2C as master
    pinMode(PIN_VALVE_SUPPLY, OUTPUT);      // Set supply valve as output
    pinMode(PIN_VALVE_IRRIGATION, OUTPUT);  // Set irrigation valve as output
    digitalWrite(PIN_VALVE_SUPPLY, LOW);    // Ensure supply valve is initially off
    digitalWrite(PIN_VALVE_IRRIGATION, LOW);// Ensure irrigation valve is initially off
   
}

// Function to start water supply
void startWaterSupply() {
  digitalWrite(PIN_VALVE_SUPPLY, HIGH);  // Open supply valve
  Serial.println("Water supply");
}

// Function to stop water supply
void stopWaterSupply() {
  digitalWrite(PIN_VALVE_SUPPLY, LOW);  // Close supply valve
  supplyActive = false;
  Serial.println("Water supply stopped.");
}

// Function to trigger fertilization on the other Arduino via I2C
void triggerFertilization() {
  Wire.beginTransmission(I2C_ADDRESS);  // Start I2C communication
  Wire.write(1);                        // Send a trigger signal (e.g., value 1)
  Wire.endTransmission();               // End transmission
  Serial.println("Fertilization trigger sent.");
  fertilizationDone = true;             // Mark fertilization as completed
}

// Function to start irrigation
void startIrrigation() {
  if (fertilizationDone) {              // Check if fertilization step is completed
    digitalWrite(PIN_VALVE_IRRIGATION, HIGH);  // Open irrigation valve
    irrigationStartTime = millis();
    irrigationActive = true;
    Serial.println("Irrigation started.");
  } else {
    Serial.println("Fertilization step not completed, irrigation delayed.");
  }
}

// Function to stop irrigation
void stopIrrigation() {
  digitalWrite(PIN_VALVE_IRRIGATION, LOW);  // Close irrigation valve
  irrigationActive = false;
  Serial.println("Irrigation stopped.");
}


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

void loop() {

     RtcDateTime now = Rtc.GetDateTime();
    // printDateTime(now);
    // Serial.println();

    if (!now.IsValid()) {
      Serial.println("RTC lost confidence in the DateTime!");
    }


   int waterLevel = analogRead(WATER_LEVEL_SENSOR);  // Read the water level sensor

  Serial.print("Water Level: ");
  Serial.print(waterLevel); Serial.print(" ");

  // Trigger water supply if level is below threshold
  if (supplyActive && waterLevel < waterLevelThreshold) {
    startWaterSupply();
  }

  // Stop water supply when target level is reached
  if (supplyActive && waterLevel >= waterLevelThreshold) {
    stopWaterSupply();
    triggerFertilization();  // Trigger fertilization via I2C
    Serial.println("Stoped water supply when target level is reached");
  }

  // Trigger irrigation if supply is completed and water level is sufficient
  if (!irrigationActive && fertilizationDone && waterLevel >= waterLevelThreshold) {
    startIrrigation();
  }

  // Stop irrigation based on manual control or other logic (e.g., timer)
  if (irrigationActive && (millis() - irrigationStartTime > 300000)) { // Example: 5 minutes
    Serial.println("Irrigation will be stoped after 5 minutes");
    stopIrrigation();
  }

  delay(5000); // Adjust the loop frequency as needed
}
