#include <WiFi.h>
#include <WebServer.h>

// Replace with your WiFi credentials
const char* ssid = "urja";
const char* password = "Urja@1234";

// Create web server on port 80
WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", "<h1>Hello from ESP32 Web Server </h1>");
}

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");
  Serial.print(" ESP32 IP address: ");
  Serial.println(WiFi.localIP());

  // Define routes
  server.on("/", handleRoot);

  // Start server
  server.begin();
  Serial.println(" Web server started!");
}

void loop() {
  server.handleClient();
}
