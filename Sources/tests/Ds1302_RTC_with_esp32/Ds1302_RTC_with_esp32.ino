#include "RTCManager.h"

RTCManager rtc;

void setup() {
    Serial.begin(9600);
    rtc.init(); 

    if (!rtc.isRTCActive()) {
        Serial.println("RTC is NOT running!");
    }
}

void loop() {
  DateTime now = rtc.getTime();
  rtc.printTimeInfo();
  delay(3000);
}
