/*********
 ESP32 DEVKIT V1

 DHT11 
 "+"   - 3.3V
 "out" - GPIO4 (D4)
 "-"   - GND

spread to save data
 https://docs.google.com/spreadsheets/d/*
*********/

#include "DHT.h"
#include "buffer_module.h"
#include "wifi_web_module.h"

// Wi-Fi credentials
const char* ssid = "kasemhub";
const char* password = "kasemhub2022";

// Google Sheets Webhook URL
const char* serverName = "https://script.google.com/macros/s/AKfycbwJffoT8peRyFgqzu0KZUfElxaVflM6zvZ0AIp50wMFgfkz8n5x-BKCI7U9vIjk1bhs/exec";

// DHT sensor settings
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

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

// Wi-Fi and Web module
WiFiWebModule wifiWeb(ssid, password, &server, &lastTemperature, &lastHumidity, &lastUpdateTime, "pool.ntp.org", 2 * 3600, 0, serverName);


//update sensor
unsigned long lastSensorUpdate = 0;
const unsigned long sensorUpdateInterval = 5000; //5 seconds

void updateSensorData() {
    unsigned long currentMillis = millis();
    if (currentMillis - lastSensorUpdate >= sensorUpdateInterval) {
        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();

        if (!isnan(temperature) && !isnan(humidity)) {
            lastTemperature = temperature;
            lastHumidity = humidity;
            lastUpdateTime = wifiWeb.getCurrentTime();
            Serial.println("Sensor data updated:");
            Serial.println("Temperature: " + String(lastTemperature) + " °C");
            Serial.println("Humidity: " + String(lastHumidity) + " %");
        } else {
            Serial.println("Failed to read from DHT sensor!");
        }
        lastSensorUpdate = currentMillis; // Обновляем время последнего чтения
    }
}


void setup() {
    Serial.begin(115200);

    // Initialize Wi-Fi and time
    wifiWeb.initializeWiFiAndTime();

    // Initialize DHT sensor
    dht.begin();

    // Set up web server handlers
    wifiWeb.setupWebHandlers();
    server.begin();
    Serial.println("Web server started!");

    // Initial sensor update
    updateSensorData();
}

void loop() {
    wifiWeb.reconnectWiFi();      // Ensure Wi-Fi is connected
    updateSensorData();           // Periodically update sensor data
    bufferModule.logScheduledData(); // Perform logging by schedule
    bufferModule.sendBufferedData(); // Send any buffered data
    server.handleClient();        // Handle HTTP requests
}
