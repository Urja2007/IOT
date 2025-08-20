#include <WiFi.h>  // Library to connect ESP32 to Wi-Fi

const char* ssid     = "urja";      // Your Wi-Fi name
const char* password = "Urja@1234";  // Your Wi-Fi password

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);               // Start Wi-Fi connection
  Serial.print("Connecting to WiFi");

  // Wait until connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());           // Print the IP address
}

void loop() {
  // Your code here
}
