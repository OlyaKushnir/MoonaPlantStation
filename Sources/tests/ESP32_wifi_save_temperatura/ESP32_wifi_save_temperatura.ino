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
#include "buffer_module.h"

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

// Buffer module
BufferModule bufferModule(serverName, &dht, logHours, loggedToday);

// Function to update sensor data
void updateSensorData() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (!isnan(temperature) && !isnan(humidity)) {
        lastTemperature = temperature;
        lastHumidity = humidity;
        lastUpdateTime = getCurrentTime();
        Serial.println("Sensor data updated:");
        Serial.println("Temperature: " + String(lastTemperature) + " °C");
        Serial.println("Humidity: " + String(lastHumidity) + " %");
    } else {
        Serial.println("Failed to read from DHT sensor!");
    }
}

void handleSave() {
    updateSensorData(); // Ensure data is updated before saving

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
    updateSensorData(); // Update data before rendering the page

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

    // Update initial sensor data
    updateSensorData();

    // Start the web server
    server.on("/", handleRoot);
    server.on("/save", HTTP_POST, handleSave);
    server.begin();
    Serial.println("Web server started!");
}

void reconnectWiFi() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Wi-Fi disconnected. Attempting to reconnect...");
        WiFi.disconnect(); // Disconnect current attempt
        WiFi.begin(ssid, password); // Start a new connection

        while (WiFi.status() != WL_CONNECTED) {
            delay(30000); // Wait 30 seconds between attempts
            Serial.println("Retrying Wi-Fi connection...");
        }

        Serial.println("\nWi-Fi reconnected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    }
}

void loop() {
    reconnectWiFi();               // Ensure Wi-Fi is connected
    bufferModule.logScheduledData(); // Perform logging by schedule
    bufferModule.sendBufferedData(); // Send any buffered data
    server.handleClient();         // Handle HTTP requests
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
