#ifndef BUFFER_MODULE_H
#define BUFFER_MODULE_H

#include <vector>
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h" // Include DHT library for logging functionality

#define MIN_FREE_MEMORY 50000 // Minimum free memory (in bytes) for stable operation

// Define the structure to store sensor data
struct SensorData {
  String date;        // Date (e.g., "2024-12-12")
  String time;        // Time (e.g., "15:30:00")
  float temperature;  // Temperature value
  float humidity;     // Humidity value
  String info;        // Additional info (optional)
};

class BufferModule {
private:
  std::vector<SensorData> dataBuffer; // Buffer to hold data
  const char* serverName;             // Server URL
  DHT* dhtSensor;                     // Pointer to DHT sensor object
  int* logHours;                      // Array of logging hours
  bool* loggedToday;                  // Array to track logged hours

public:
  // Constructor to initialize the buffer module with the server name and DHT sensor
  BufferModule(const char* serverName, DHT* dhtSensor, int* logHours, bool* loggedToday)
      : serverName(serverName), dhtSensor(dhtSensor), logHours(logHours), loggedToday(loggedToday) {}

  // Check if there is enough free memory to add data to the buffer
  bool hasEnoughMemory() {
    size_t freeMemory = ESP.getFreeHeap(); // Get the amount of free heap memory
    if (freeMemory < MIN_FREE_MEMORY) {
      Serial.println("Not enough free memory to add new data to the buffer.");
      return false;
    }
    return true;
  }

  // Add data to the buffer
  void addToBuffer(float temperature, float humidity, String info) {
    if (!hasEnoughMemory()) {
      return; // Skip adding data if memory is too low
    }

    String currentTime = getCurrentTime();
    SensorData newData = {
        currentTime.substring(0, 10), // Date
        currentTime.substring(11, 19), // Time
        temperature,
        humidity,
        info};

    dataBuffer.push_back(newData);
    Serial.println("Data added to buffer.");
    Serial.print("Current free memory: ");
    Serial.println(ESP.getFreeHeap());
  }

  // Send buffered data to the server
  void sendBufferedData() {
    if (WiFi.status() == WL_CONNECTED && !dataBuffer.empty()) {
      Serial.println("Sending buffered data...");
      HTTPClient http;

      for (auto it = dataBuffer.begin(); it != dataBuffer.end();) {
        String jsonPayload = "{\"date\":\"" + it->date +
                             "\",\"time\":\"" + it->time +
                             "\",\"temperature\":" + String(it->temperature) +
                             ",\"humidity\":" + String(it->humidity) +
                             ",\"info\":\"" + it->info + "\"}";

        http.begin(serverName); // Use the serverName provided in the constructor
        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.POST(jsonPayload);
        http.end();

        if (httpResponseCode > 0) {
          Serial.println("Buffered data sent successfully.");
          it = dataBuffer.erase(it); // Remove successfully sent data from buffer
        } else {
          Serial.println("Failed to send buffered data.");
          break; // Stop sending if there's an error
        }
      }
    }
  }

  // Log data by schedule
  void logScheduledData() {
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      int currentHour = timeinfo.tm_hour;

      for (int i = 0; i < 4; i++) {
        if (currentHour == logHours[i] && !loggedToday[i]) {
          float temperature = dhtSensor->readTemperature();
          float humidity = dhtSensor->readHumidity();
          String info = "Logged by scheduler";

          if (!isnan(temperature) && !isnan(humidity)) {
            // Always add data to the buffer
            addToBuffer(temperature, humidity, info);
          } else {
            Serial.println("Failed to read from DHT sensor!");
          }

          loggedToday[i] = true; // Mark this time slot as logged
        }

        // Reset flags at midnight
        if (currentHour == 0) {
          loggedToday[i] = false;
        }
      }
    }
  }

  // Get the current time as a string
  String getCurrentTime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      return "Failed to obtain time!";
    }

    char timeString[30];
    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return String(timeString);
  }
};

#endif // BUFFER_MODULE_H
