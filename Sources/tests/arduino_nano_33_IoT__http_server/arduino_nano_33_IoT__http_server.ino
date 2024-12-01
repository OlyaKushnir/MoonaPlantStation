#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

// Wi-Fi credentials
const char* ssid = "kasemhub";
const char* password = "kasemhub2022";

// Set up NTP Client to get time
WiFiUDP udp;
NTPClient timeClient(udp, "pool.ntp.org", 0, 60000);  // Update time every 60 seconds (0 is UTC)

// Server variables
WiFiServer server(80); // Web server on port 80

void setup() {
  // Start serial communication for debugging
  Serial.begin(9600);

  // Connect to Wi-Fi
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Start the NTP client
  timeClient.begin();

  // Start the web server
  server.begin();
  Serial.println("Web server started");

  // Give a brief delay for the web server to start properly
  delay(1000);
}

void loop() {
  // Update the NTP client to get the latest time
  timeClient.update();

  // Check for incoming client requests
  WiFiClient client = server.available();
  if (client) {
    String currentTime = timeClient.getFormattedTime(); // Get the formatted time

    // Wait for the client to send data
    while (client.connected()) {
      if (client.available()) {
        // Read the incoming request from the client
        String request = client.readStringUntil('\r');
        Serial.println(request);
        
        // Send an HTTP response
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();

        // Serve the HTML page with the current time
        client.println("<!DOCTYPE HTML><html>");
        client.println("<head><title>Time Now</title></head>");
        client.println("<body>");
        client.println("<h1>Current Time: </h1>");
        client.println("<p>" + currentTime + "</p>");
        client.println("</body></html>");

        // Close the connection
        client.stop();
      }
    }
  }
}
