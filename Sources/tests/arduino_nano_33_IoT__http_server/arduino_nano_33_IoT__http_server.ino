#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

// Wi-Fi credentials
// const char* ssid = "kasemhub";
// const char* password = "kasemhub2022";
const char* ssid = "Olya iPhone 14";
const char* password = "1122334455";

// Set up NTP Client to get the current time
WiFiUDP udp;
NTPClient timeClient(udp, "pool.ntp.org", 0, 60000);  // Update time every 60 seconds (0 is UTC)

// Web server on port 80
WiFiServer server(80);

void setup() {
  Serial.begin(9600);

  // Check if the Wi-Fi module is present
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("Wi-Fi module not present.");
    while (true);
  }

  // Connect to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  unsigned long startMillis = millis();
  unsigned long timeout = 30000; // 30 seconds timeout

  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - startMillis > timeout) {
      Serial.println("Failed to connect to Wi-Fi.");
      return;
    }
    delay(500);
    Serial.print(".");
  }

  // Connected to Wi-Fi
  Serial.println("Connected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());  // Print the IP address assigned to the Arduino
  Serial.println(WiFi.status());

  // Start the NTP client to get time
  timeClient.begin();

  // Start the web server
  server.begin();
  Serial.println("Web server started!");
}

void loop() {
  // Check Wi-Fi connection every 5 seconds
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi connection lost! Attempting to reconnect...");
    
    // Attempt to reconnect
    int reconnectAttempts = 0;
    while (WiFi.status() != WL_CONNECTED && reconnectAttempts < 10) {
      reconnectAttempts++;
      Serial.print("Reconnection attempt: ");
      Serial.println(reconnectAttempts);
      
      WiFi.begin(ssid, password);
      unsigned long startMillis = millis();
      
      // Wait for connection
      while (WiFi.status() != WL_CONNECTED && millis() - startMillis < 10000) {
        delay(500);  // Wait for 0.5 seconds before next check
        Serial.print(".");
      }

      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Reconnected to Wi-Fi!");
        Serial.print("New IP Address: ");
        Serial.println(WiFi.localIP());
        break;  // Exit loop after successful connection
      } else {
        Serial.println("Failed to reconnect. Retrying...");
      }
    }
    
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Failed to reconnect after 10 attempts.");
    }
  }

  // Update the NTP client to get the current time
  timeClient.update();

  // Check if there's an incoming client request
  WiFiClient client = server.available();
  if (client) {
    String currentTime = timeClient.getFormattedTime(); // Get the current time as a string

    // Wait for the client to send data
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r');
        Serial.println(request);

        // Send the HTTP response headers
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();

        // Serve an HTML page with the current time
        client.println("<!DOCTYPE HTML><html>");
        client.println("<head><title>Time Now</title></head>");
        client.println("<body>");
        client.println("<h1>Current Time:</h1>");
        client.println("<p>" + currentTime + "</p>");
        client.println("</body></html>");

        // Close the connection
        client.stop();
      }
    }
  }

  delay(10000);  // Delay before checking connection again
}
