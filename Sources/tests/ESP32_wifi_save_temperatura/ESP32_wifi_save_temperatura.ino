/*********
 ESP32 DEVKIT V1

 DHT11 
 "+"   - 3.3V
 "out" - GPIO4 (D4)
 "-"   - GND
*********/

#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include "DHT.h"
#include <time.h>

// Wi-Fi credentials
const char* ssid = "kasemhub";
const char* password = "kasemhub2022";

// Google Sheets Webhook URL
const char* serverName = "https://script.google.com/macros/s/AKfycbwHqUQTTQqKxp4f70E0FibWiC5kGuTgJfRIwZduRrUsfxpHI4BVafQiaMGPHb28tg4r/exec";

// DHT sensor settings
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// NTP server settings
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 2 * 3600; // GMT+2
const int daylightOffset_sec = 0;

// Web server
WebServer server(80);

// Data variables
float lastTemperature = NAN;
float lastHumidity = NAN;
String lastUpdateTime = "Never";

// Logging schedule
int logHours[4] = {8, 12, 17, 2};
bool loggedToday[4] = {false, false, false, false};

void logScheduledData() {
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    int currentHour = timeinfo.tm_hour;

    for (int i = 0; i < 4; i++) {
      if (currentHour == logHours[i] && !loggedToday[i]) {
        // Send data to Google Sheets
        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();
        String currentTime = getCurrentTime();

        if (!isnan(temperature) && !isnan(humidity)) {
          String jsonPayload = "{\"date\":\"" + currentTime.substring(0, 10) +
                               "\",\"time\":\"" + currentTime.substring(11, 19) +
                               "\",\"temperature\":" + String(temperature) +
                               ",\"humidity\":" + String(humidity) +
                               ",\"info\":\"Logged by scheduler\"}";

          HTTPClient http;
          http.begin(serverName);
          http.addHeader("Content-Type", "application/json");
          int httpResponseCode = http.POST(jsonPayload);
          http.end();

          if (httpResponseCode > 0) {
            Serial.println("Logged data successfully at " + String(currentHour) + ":00.");
          } else {
            Serial.println("Error logging data!");
          }
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

void handleSave() {
  if (server.hasArg("info")) {
    String info = server.arg("info");

    // Ensure temperature and humidity have valid values
    float temperature = isnan(lastTemperature) ? 0.0 : lastTemperature;
    float humidity = isnan(lastHumidity) ? 0.0 : lastHumidity;
    String currentTime = getCurrentTime();

    String jsonPayload = "{\"date\":\"" + currentTime.substring(0, 10) + 
                         "\",\"time\":\"" + currentTime.substring(11, 19) +
                         "\",\"temperature\":" + String(temperature) +
                         ",\"humidity\":" + String(humidity) +
                         ",\"info\":\"" + info + "\"}";

    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(jsonPayload);

    String responseMessage = "";
    if (httpResponseCode > 0) {
      responseMessage = "Data sent successfully!";
    } else {
      responseMessage = "Error sending data!";
    }
    http.end();

    String html = "<html><head><title>Moona Plant Station</title></head><body>";
    html += "<h1>Moona Plant Station</h1>";
    html += "<p style='color:green;'>" + responseMessage + "</p>";
    html += "<a href='/'>Return to Main Page</a>";
    html += "</body></html>";

    server.send(200, "text/html", html);
  } else {
    server.send(400, "text/html", "<html><body><h1>Bad Request: Missing 'info'</h1></body></html>");
  }
}

void handleRoot() {
  String html = "<html><head><title>Moona Plant Station</title></head><body>";
  html += "<h1>Moona Plant Station</h1>";
  html += "<p>Last Update: " + lastUpdateTime + "</p>";
  html += "<p>Temperature: " + (isnan(lastTemperature) ? "N/A" : String(lastTemperature) + " °C") + "</p>";
  html += "<p>Humidity: " + (isnan(lastHumidity) ? "N/A" : String(lastHumidity) + " %") + "</p>";
  html += "<form action='/save' method='POST'>";
  html += "<label for='info'>Additional Info:</label>";
  html += "<input type='text' id='info' name='info' maxlength='100'><br><br>";
  html += "<input type='submit' value='Save Data'>";
  html += "</form>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void sensorTask(void* parameter) {
  while (true) {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (!isnan(temperature) && !isnan(humidity)) {
      lastTemperature = temperature;
      lastHumidity = humidity;
      lastUpdateTime = getCurrentTime();
      Serial.println("Updated data:");
      Serial.println("Temperature: " + String(lastTemperature) + " °C");
      Serial.println("Humidity: " + String(lastHumidity) + " %");
    } else {
      Serial.println("Failed to read from DHT sensor!");
    }

    logScheduledData();
    delay(120000); // Update every 2 minutes
  }
}

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  dht.begin();

  // Synchronize time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Time synchronization complete");

  // Start the web server on Core 0
  server.on("/", handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.begin();
  Serial.println("Web server started!");

  // Start sensor task on Core 1
  xTaskCreatePinnedToCore(sensorTask, "SensorTask", 10000, NULL, 1, NULL, 1);
}

void loop() {
  server.handleClient();
}

String getCurrentTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "Failed to obtain time!";
  }

  char timeString[30];
  strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(timeString);
}
