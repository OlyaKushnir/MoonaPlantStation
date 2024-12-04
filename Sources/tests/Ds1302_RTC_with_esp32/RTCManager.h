#ifndef RTCMANAGER_H
#define RTCMANAGER_H

// this module uses the library "RTCLib by NeiroN v1.6.3"
// https://github.com/lab-mathias-claussen/RTCLib-NeiroNx

#include <Wire.h>
#include <RTClib.h>

// Define the pin numbers for the RTC (can be adjusted in the .cpp file)
#define RTC_RST_PIN 2
#define RTC_CLK_PIN 4
#define RTC_DAT_PIN 5

class RTCManager {
public:
    // Constructor: Initializes the RTC with specified pins
    RTCManager(uint8_t rstPin = RTC_RST_PIN, uint8_t clkPin = RTC_CLK_PIN, uint8_t datPin = RTC_DAT_PIN);

    // Initializes the RTC
    void init();

    // Gets the current time from the RTC
    DateTime getTime();

    // Adjusts the RTC time (sets it to the current compile-time)
    void adjustTimeToCompileTime();

    // Checks if the RTC is running
    bool isRTCActive();

    // Returns the current time in a string format
    const char* getTimeAsString();

    // Prints the current time information to the Serial Monitor
    void printTimeInfo();

private:
    // Internal instance of DS1302 RTC object
    DS1302 rtc;
    
    // Buffer for DateTime.toString()
    char buf[20];
};

#endif // RTCMANAGER_H
