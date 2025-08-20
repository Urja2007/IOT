#include <WiFi.h>

void setup() {
  Serial.begin(115200);

  // Set ESP32 in Station mode (required to get MAC)
  WiFi.mode(WIFI_STA);

  // Print MAC Address
  Serial.print("ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
}
