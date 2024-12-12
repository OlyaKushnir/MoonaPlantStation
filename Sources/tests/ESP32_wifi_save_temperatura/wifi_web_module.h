#ifndef WIFI_WEB_MODULE_H
#define WIFI_WEB_MODULE_H

#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <time.h>

class WiFiWebModule {
private:
    const char* ssid;
    const char* password;
    WebServer* server;
    float* lastTemperature;
    float* lastHumidity;
    String* lastUpdateTime;
    const char* ntpServer;
    const long gmtOffset_sec;
    const int daylightOffset_sec;
    const char* serverName;

public:
    WiFiWebModule(const char* ssid, const char* password, WebServer* server, 
                  float* lastTemperature, float* lastHumidity, String* lastUpdateTime, 
                  const char* ntpServer, long gmtOffset_sec, int daylightOffset_sec,
                  const char* serverName)
        : ssid(ssid), password(password), server(server), lastTemperature(lastTemperature),
          lastHumidity(lastHumidity), lastUpdateTime(lastUpdateTime), ntpServer(ntpServer),
          gmtOffset_sec(gmtOffset_sec), daylightOffset_sec(daylightOffset_sec), serverName(serverName) {}

    void reconnectWiFi() {
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("Wi-Fi disconnected. Attempting to reconnect...");
            WiFi.disconnect();
            WiFi.begin(ssid, password);

            while (WiFi.status() != WL_CONNECTED) {
                delay(30000); // Wait 30 seconds between attempts
                Serial.println("Retrying Wi-Fi connection...");
            }

            Serial.println("\nWi-Fi reconnected!");
            Serial.print("IP Address: ");
            Serial.println(WiFi.localIP());
        }
    }

    void setupWebHandlers() {
        server->on("/", [this]() { handleRoot(); });
        server->on("/save", HTTP_POST, [this]() { handleSave(); });
    }

    void handleRoot() {
        String html = "<html><head><title>Moona Plant Station</title></head><body>";
        html += "<h1>Moona Plant Station</h1>";
        html += "<p>Last Update: " + *lastUpdateTime + "</p>";
        html += "<p>Temperature: " + (isnan(*lastTemperature) ? "N/A" : String(*lastTemperature) + " °C") + "</p>";
        html += "<p>Humidity: " + (isnan(*lastHumidity) ? "N/A" : String(*lastHumidity) + " %") + "</p>";
        html += "<form action='/save' method='POST'>";
        html += "<label for='info'>Additional Info:</label>";
        html += "<input type='text' id='info' name='info' maxlength='100'><br><br>";
        html += "<input type='submit' value='Save Data'>";
        html += "</form>";
        html += "</body></html>";

        server->send(200, "text/html", html);
    }

    void handleSave() {
        if (server->hasArg("info")) {
            String info = server->arg("info");

            // Ensure temperature and humidity have valid values
            float temperature = isnan(*lastTemperature) ? 0.0 : *lastTemperature;
            float humidity = isnan(*lastHumidity) ? 0.0 : *lastHumidity;
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

            server->send(200, "text/html", html);
        } else {
            server->send(400, "text/html", "<html><body><h1>Bad Request: Missing 'info'</h1></body></html>");
        }
    }

    void initializeWiFiAndTime() {
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println("\nWiFi connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());

        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        Serial.println("Time synchronization complete");
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
};

#endif // WIFI_WEB_MODULE_H
