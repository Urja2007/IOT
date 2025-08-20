#include <HardwareSerial.h>   // Library for using extra UART ports

// Create a HardwareSerial object named ReceiverSerial using UART1
HardwareSerial ReceiverSerial(1);

void setup() {
  Serial.begin(115200); 
  // Data format = 8 data bits, no parity, 1 stop bit (SERIAL_8N1)
  // RX pin = 16 (connect to Sender’s TX)
  // TX pin = 17 (not used in this receiver-only setup, but required for begin)
  ReceiverSerial.begin(9600, SERIAL_8N1, 16, 17);
}

void loop() {
  // Check if data is available on UART1
  if (ReceiverSerial.available()) {
    // Read incoming string until newline '\n'
    String msg = ReceiverSerial.readStringUntil('\n');

    // Print the received message on Serial Monitor
    Serial.print("Received: ");
    Serial.println(msg);
  }
}
