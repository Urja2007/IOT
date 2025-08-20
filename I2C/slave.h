#include <Wire.h>   // Library for I2C communication

#define SLAVE_ADDR 0x08   // I2C address of this ESP (must match master's target address)


// Callback function that runs whenever data is received from Master
void receiveEvent(int numBytes) {
  while (Wire.available()) {       // Loop while data is available
    char c = Wire.read();          // Read one byte (character) from I2C buffer
    Serial.print(c);               // Print the received character
  }
  Serial.println();                // Print newline after full message
}

void setup() {
  Wire.begin(SLAVE_ADDR);          // Initialize ESP as an I2C Slave with given address
  Wire.onReceive(receiveEvent);    // Register callback for when data is received
  Serial.begin(115200);            // Start serial monitor for debugging
}

void loop() {
  // Nothing here; data reception handled asynchronously in receiveEvent()
}
