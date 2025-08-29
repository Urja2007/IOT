#define MQ135_PIN 34   // ADC pin where MQ135 AOUT is connected
#define RL 10000.0     // Load resistance in ohms (10k typical)
#define Vcc 5.0        // Sensor supply voltage (if using 5V board)

// Calibration value (R0) — must be determined in clean air
// Default approx value, but you should calibrate it!
float R0 = 10000.0;  

void setup() {
  Serial.begin(9600);
  Serial.println("MQ135 Air Quality Sensor");
}

void loop() {
  int adcValue = analogRead(MQ135_PIN);
  float Vout = adcValue * (3.3 / 4095.0);  // ESP32 ADC to voltage (3.3V ref)
  
  // Calculate sensor resistance Rs
  float Rs = (Vcc * RL / Vout) - RL;

  // Ratio Rs/R0
  float ratio = Rs / R0;

  // Example: CO2 curve (approximation, from datasheet log-log graph)
  // ppm = a * (ratio ^ -b)
  // Constants depend on gas. For CO2, approximation:
  float a = 116.6020682;
  float b = 2.769034857;
  float ppm = a * pow(ratio, -b);

  // Print results
  Serial.print("ADC: "); Serial.print(adcValue);
  Serial.print(" | Vout: "); Serial.print(Vout, 3); Serial.print(" V");
  Serial.print(" | Rs: "); Serial.print(Rs, 1); Serial.print(" ohm");
  Serial.print(" | Ratio: "); Serial.print(ratio, 2);
  Serial.print(" | CO2: "); Serial.print(ppm, 1); Serial.println(" ppm");

  delay(1000);
}
