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
int logHours[4] = {8, 12, 17, 2}; // Log at 08:00, 12:00, 17:00, 02:00
bool loggedToday[4] = {false, false, false, false}; // Flags to track logging

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

    // Send temporary feedback with delay
    String html = "<html><head><title>Moona Plant Station</title>";
    html += "<meta http-equiv='refresh' content='2;url=/'></head><body>";
    html += "<h1>Moona Plant Station</h1>";
    html += "<p style='color:green;'>" + responseMessage + "</p>";
    html += "</body></html>";

    server.send(200, "text/html", html);
  } else {
    server.send(400, "text/html", "<html><body><h1>Bad Request: Missing 'info'</h1></body></html>");
  }
}

void handleRoot() {
  String html = "<html><head><title>Moona Plant Station</title>";
  html += "<script>";
  html += "function handleSubmit(event) {";
  html += "  event.preventDefault();"; // Prevent form submission
  html += "  const form = event.target;";
  html += "  const info = form.info.value;";
  html += "  fetch('/save', {";
  html += "    method: 'POST',";
  html += "    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },";
  html += "    body: `info=${encodeURIComponent(info)}`";
  html += "  }).then(response => response.text()).then(alert).catch(console.error);";
  html += "  form.reset();"; // Reset form after submission
  html += "}";
  html += "</script>";
  html += "</head><body>";
  html += "<h1>Moona Plant Station</h1>";
  html += "<p>Last Update: " + lastUpdateTime + "</p>";
  html += "<p>Temperature: " + (isnan(lastTemperature) ? "N/A" : String(lastTemperature) + " °C") + "</p>";
  html += "<p>Humidity: " + (isnan(lastHumidity) ? "N/A" : String(lastHumidity) + " %") + "</p>";
  html += "<form onsubmit='handleSubmit(event)'>";
  html += "<label for='info'>Additional Info:</label>";
  html += "<input type='text' id='info' name='info' maxlength='100'><br><br>";
  html += "<input type='submit' value='Save Data'>";
  html += "</form>";
  html += "</body></html>";

  server.send(200, "text/html", html);
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

  // Initialize DHT sensor
  dht.begin();

  // Synchronize time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Time synchronization complete");

  // Initial reading from DHT sensor
  lastTemperature = dht.readTemperature();
  lastHumidity = dht.readHumidity();

  if (!isnan(lastTemperature) && !isnan(lastHumidity)) {
    lastUpdateTime = getCurrentTime();

    // Log data for "web server restart"
    String jsonPayload = "{\"date\":\"" + lastUpdateTime.substring(0, 10) +
                         "\",\"time\":\"" + lastUpdateTime.substring(11, 19) +
                         "\",\"temperature\":" + String(lastTemperature) +
                         ",\"humidity\":" + String(lastHumidity) +
                         ",\"info\":\"Web server restart\"}";

    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(jsonPayload);
    http.end();

    if (httpResponseCode > 0) {
      Serial.println("Web server restart data logged successfully.");
    } else {
      Serial.println("Error logging web server restart data!");
    }

    Serial.println("Initial data read:");
    Serial.println("Temperature: " + String(lastTemperature) + " °C");
    Serial.println("Humidity: " + String(lastHumidity) + " %");
    Serial.println("Last Update Time: " + lastUpdateTime);
  } else {
    Serial.println("Failed to read from DHT sensor at startup!");
    lastTemperature = 0.0; // Default value
    lastHumidity = 0.0;    // Default value
    lastUpdateTime = "Never";
  }

  // Start the web server
  server.on("/", handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.begin();
  Serial.println("Web server started!");
}

void loop() {
  static unsigned long lastUpdate = 0;
  static bool firstLoop = true; // Prevent double logging on restart
  unsigned long currentMillis = millis();

  if (firstLoop) {
    firstLoop = false;
    return;
  }

  // Periodic update every 60 seconds
  if (currentMillis - lastUpdate >= 60000) {
    lastUpdate = currentMillis;

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (!isnan(temperature) && !isnan(humidity)) {
      lastTemperature = temperature;
      lastHumidity = humidity;
      lastUpdateTime = getCurrentTime();
      Serial.println("Updated data:");
      Serial.println("Temperature: " + String(lastTemperature) + " °C");
      Serial.println("Humidity: " + String(lastHumidity) + " %");
      Serial.println("Last Update Time: " + lastUpdateTime);
    } else {
      Serial.println("Failed to read from DHT sensor!");
    }
  }

  // Check for scheduled logging
  logScheduledData();

  // Handle client requests
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
