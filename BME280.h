#include <Wire.h>

#define BME280_ADDRESS 0x76   // Change to 0x77 if SDO pin tied to VCC
#define SEALEVELPRESSURE_HPA (1013.25)

int32_t t_fine;

// Calibration data
uint16_t dig_T1;
int16_t dig_T2, dig_T3;
uint16_t dig_P1;
int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
uint8_t dig_H1, dig_H3;
int16_t dig_H2, dig_H4, dig_H5, dig_H6;

void writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(BME280_ADDRESS);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

uint8_t read8(uint8_t reg) {
  Wire.beginTransmission(BME280_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(BME280_ADDRESS, 1);
  return Wire.read();
}

uint16_t read16(uint8_t reg) {
  Wire.beginTransmission(BME280_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(BME280_ADDRESS, 2);
  uint16_t value = Wire.read();
  value |= (uint16_t)Wire.read() << 8;
  return value;
}

int16_t readS16(uint8_t reg) {
  return (int16_t)read16(reg);
}

uint32_t read24(uint8_t reg) {
  Wire.beginTransmission(BME280_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(BME280_ADDRESS, 3);
  uint32_t value = Wire.read();
  value |= (uint32_t)Wire.read() << 8;
  value |= (uint32_t)Wire.read() << 16;
  return value;
}

void readCalibrationData() {
  dig_T1 = read16(0x88);
  dig_T2 = readS16(0x8A);
  dig_T3 = readS16(0x8C);

  dig_P1 = read16(0x8E);
  dig_P2 = readS16(0x90);
  dig_P3 = readS16(0x92);
  dig_P4 = readS16(0x94);
  dig_P5 = readS16(0x96);
  dig_P6 = readS16(0x98);
  dig_P7 = readS16(0x9A);
  dig_P8 = readS16(0x9C);
  dig_P9 = readS16(0x9E);

  dig_H1 = read8(0xA1);
  dig_H2 = readS16(0xE1);
  dig_H3 = read8(0xE3);
  dig_H4 = (read8(0xE4) << 4) | (read8(0xE5) & 0x0F);
  dig_H5 = (read8(0xE6) << 4) | (read8(0xE5) >> 4);
  dig_H6 = (int8_t)read8(0xE7);
}

float readTemperature() {
  int32_t adc_T = read24(0xFA) >> 4;

  int32_t var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
  int32_t var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) * ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) *
                  ((int32_t)dig_T3)) >> 14;
  t_fine = var1 + var2;

  float T = (t_fine * 5 + 128) >> 8;
  return T / 100.0;
}

float readPressure() {
  int32_t adc_P = read24(0xF7) >> 4;

  int64_t var1 = ((int64_t)t_fine) - 128000;
  int64_t var2 = var1 * var1 * (int64_t)dig_P6;
  var2 = var2 + ((var1 * (int64_t)dig_P5) << 17);
  var2 = var2 + (((int64_t)dig_P4) << 35);
  var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8) + ((var1 * (int64_t)dig_P2) << 12);
  var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)dig_P1) >> 33;

  if (var1 == 0) return 0; // avoid division by zero

  int64_t p = 1048576 - adc_P;
  p = (((p << 31) - var2) * 3125) / var1;
  var1 = (((int64_t)dig_P9) * (p >> 13) * (p >> 13)) >> 25;
  var2 = (((int64_t)dig_P8) * p) >> 19;

  p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7) << 4);
  return (float)p / 256.0 / 100.0; // hPa
}

float readHumidity() {
  int32_t adc_H = read16(0xFD);

  int32_t v_x1_u32r;
  v_x1_u32r = (t_fine - ((int32_t)76800));
  v_x1_u32r = (((((adc_H << 14) - (((int32_t)dig_H4) << 20) -
                  (((int32_t)dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
               (((((((v_x1_u32r * ((int32_t)dig_H6)) >> 10) *
                    (((v_x1_u32r * ((int32_t)dig_H3)) >> 11) + ((int32_t)32768))) >> 10) +
                  ((int32_t)2097152)) * ((int32_t)dig_H2) + 8192) >> 14));
  v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t)dig_H1)) >> 4));
  v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
  v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
  return (v_x1_u32r >> 12) / 1024.0;
}

float readAltitude(float seaLevelhPa = SEALEVELPRESSURE_HPA) {
  float pressure = readPressure(); // hPa
  return 44330.0 * (1.0 - pow(pressure / seaLevelhPa, 0.1903));
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  // Reset BME280
  writeRegister(0xE0, 0xB6);
  delay(300);

  // Read calibration
  readCalibrationData();

  // Humidity oversampling ×1
  writeRegister(0xF2, 0x01);
  // Normal mode, temp & pressure oversampling ×1
  writeRegister(0xF4, 0x27);
  // Standby 0.5ms, filter off
  writeRegister(0xF5, 0xA0);

  Serial.println("BME280 initialized without libraries!");
}

void loop() {
  float temp = readTemperature();
  float press = readPressure();
  float hum = readHumidity();
  float alt = readAltitude();

  Serial.print("Temperature = ");
  Serial.print(temp);
  Serial.println(" °C");

  Serial.print("Pressure = ");
  Serial.print(press);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(alt);
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(hum);
  Serial.println(" %");

  Serial.println();
  delay(2000);
}
