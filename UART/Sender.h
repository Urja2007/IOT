#include <HardwareSerial.h>   // Library for using extra UART ports

// Create a HardwareSerial object named SenderSerial using UART1
HardwareSerial SenderSerial(1);  

void setup() {
  Serial.begin(115200);   
  // Data format = 8 data bits, no parity, 1 stop bit (SERIAL_8N1)
  // RX pin = 16, TX pin = 17  (you can change as per wiring)
  SenderSerial.begin(9600, SERIAL_8N1, RX, TX); 
}

void loop() {
  // Send message through UART1 (SenderSerial) to connected device
  SenderSerial.println("Hello !");     

  // Print confirmation on Serial Monitor (USB debug)
  Serial.println("Message sent.");     

  delay(1000);  // Wait 1 second before sending again
}
