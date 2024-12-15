//@ -0,0 +1,152 @@
/*********
 ESP32 DEVKIT V1

 DHT11 
 "+"   - 3.3V
 "out" - GPIO4 
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
const char* serverName = "https://script.google.com/macros/s/AKfycbxyZ_Iz0bYnnkChQnpE7hhTSOp25CKwN8btkcl_LkxT5QA0OULEJJ5P3y2XwMyiKzSJ/exec";

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

// Schedule times for data logging
int logTimes[4] = {8, 12, 17, 2}; // 08:00, 12:00, 17:00, 02:00
bool loggedToday[4] = {false, false, false, false}; // To track if data is logged

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  dht.begin();

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Time synchronization complete");

  // Set up the web server
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server started!");
}

void loop() {
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    int currentHour = timeinfo.tm_hour;

    // Check scheduled times
    for (int i = 0; i < 4; i++) {
      if (currentHour == logTimes[i] && !loggedToday[i]) {
        logData();
        loggedToday[i] = true;
      }

      // Reset flags at midnight
      if (currentHour == 0) {
        loggedToday[i] = false;
      }
    }
  }

  server.handleClient(); // Handle web server requests
  delay(1000);
}

void logData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Read sensor data
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (!isnan(temperature) && !isnan(humidity)) {
      // Update the latest data
      lastTemperature = temperature;
      lastHumidity = humidity;
      lastUpdateTime = getCurrentTime();

      // Prepare payload for Google Sheets
      String jsonPayload = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + "}";
      http.begin(serverName);
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST(jsonPayload);

      if (httpResponseCode > 0) {
        Serial.println("Data sent successfully!");
      } else {
        Serial.print("Error sending data: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    } else {
      Serial.println("Failed to read from DHT sensor!");
    }
  } else {
    Serial.println("WiFi disconnected!");
  }
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

void handleRoot() {
  // Prepare HTML response
  String html = "<html><head><title>MoOona Plant Station</title>";
  html += "<meta http-equiv='refresh' content='10'>"; // Auto-refresh every 10 seconds
  html += "</head><body>";
  html += "<h1>Moona Plant Station</h1>";
  html += "<p>Last Update: " + lastUpdateTime + "</p>";
  html += "<p>Temperature: ";
  html += isnan(lastTemperature) ? "N/A" : String(lastTemperature) + " °C";
  html += "</p><p>Humidity: ";
  html += isnan(lastHumidity) ? "N/A" : String(lastHumidity) + " %";
  html += "</p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}