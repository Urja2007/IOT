#include <Wire.h>   // Library for I2C communication

#define SLAVE_ADDR 0x08   // I2C address of the slave ESP32

void setup() {
  Wire.begin();          // Initialize I2C as Master
  Serial.begin(115200);  // Start serial communication for debugging
}

void loop() {
  // Example data to send
  String message = "Hello ESP!";

  // Start transmission to the slave device
  Wire.beginTransmission(SLAVE_ADDR);

  // Write the message as bytes to the slave
  // (const uint8_t*) converts String to a byte array
  // message.length() gives the number of bytes to send
  Wire.write((const uint8_t*)message.c_str(), message.length());

  // End the transmission (actually sends the data to slave)
  Wire.endTransmission();

  // Print confirmation on Serial Monitor
  Serial.println("Message sent to Slave: " + message);

  delay(2000);  // Wait for 2 seconds before sending next message
}
