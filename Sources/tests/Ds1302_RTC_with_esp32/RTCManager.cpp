#include "RTCManager.h"

// Constructor: Initializes the RTC with specified pins
RTCManager::RTCManager(uint8_t rstPin, uint8_t clkPin, uint8_t datPin) 
    : rtc(rstPin, clkPin, datPin) {}

// Initializes the RTC
void RTCManager::init() {
    rtc.begin();
    
    if (!rtc.isrunning()) {
        Serial.println("RTC is NOT running!");
        adjustTimeToCompileTime();
    }
}

// Adjusts the RTC time (sets it to the current compile-time)
void RTCManager::adjustTimeToCompileTime() {
    rtc.adjust(DateTime(__DATE__, __TIME__));
}

// Gets the current time from the RTC
DateTime RTCManager::getTime() {
    return rtc.now();
}

// Checks if the RTC is running
bool RTCManager::isRTCActive() {
    return rtc.isrunning();
}

// Returns the current time in a string format
const char* RTCManager::getTimeAsString() {
    DateTime now = getTime();
    return now.tostr(buf);
}

// Prints the current time information to the Serial Monitor
void RTCManager::printTimeInfo() {
    DateTime now = getTime();

    Serial.println(getTimeAsString());
    Serial.print(" since midnight 1970/01/01 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");

    // Calculate a date which is 7 days and 30 seconds into the future
    DateTime future(now + (7 * 86400L + 30));
    Serial.print(" now + 7d + 30s: ");
    Serial.println(future.tostr(buf));

    // Calculate a date which is 30 days before
    DateTime past(now - TimeDelta(30 * 86400L));
    Serial.print(" now - 30d: ");
    Serial.println(past.tostr(buf));

    Serial.println();
}
