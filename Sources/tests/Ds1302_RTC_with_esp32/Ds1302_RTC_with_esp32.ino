#include "RTCManager.h"

RTCManager rtc;

void setup() {
    Serial.begin(9600);
    rtc.init(); 

    if (!rtc.isRTCActive()) {
        Serial.println("RTC is NOT running!");
    }
}

DateTime start;
void loop() {
  DateTime now = rtc.getTime();

  static char buf[20];
  TimeDelta firstEventTimeStamp(7, 0, 0, 0);
  Serial.print("7 days in the furture:");
  Serial.println((now + firstEventTimeStamp).tostr(buf));

  rtc.printTimeInfo();
  delay(3000);
}
