#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include "Adafruit_TCS34725.h"

// Pin sensor warna TCS34725 (I2C)
#define SDA_PIN D2
#define SCL_PIN D3

// Pin LoRa SX1278
#define SS    D8
#define RST   D1
#define DIO0  D0

// Inisialisasi sensor warna
Adafruit_TCS34725 tcs = Adafruit_TCS34725(
  TCS34725_INTEGRATIONTIME_614MS,
  TCS34725_GAIN_1X);

// Variabel penghitung paket
int counter = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!tcs.begin()) {
    Serial.println("Sensor tidak terdeteksi!");
    while (1);
  }

  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa gagal dimulai!");
    while (1);
  }

  Serial.println("Setup selesai.");
}

void loop() {
  uint16_t r_raw, g_raw, b_raw, c;
  tcs.getRawData(&r_raw, &g_raw, &b_raw, &c);

  // Hindari pembagian dengan nol
  if (c == 0) c = 1;

  // Normalisasi RGB ke skala 0–255
  uint8_t r = (r_raw * 255) / c;
  uint8_t g = (g_raw * 255) / c;
  uint8_t b = (b_raw * 255) / c;

  // Debug ke Serial Monitor
  Serial.print("Raw R: "); Serial.print(r_raw);
  Serial.print(" G: "); Serial.print(g_raw);
  Serial.print(" B: "); Serial.print(b_raw);
  Serial.print(" C: "); Serial.println(c);

  Serial.print("Norm R: "); Serial.print(r);
  Serial.print(" G: "); Serial.print(g);
  Serial.print(" B: "); Serial.println(b);

  // Format data: counter,R,G,B
  String dataToSend = String(counter) + "," + String(r) + "," + String(g) + "," + String(b);

  // Kirim data via LoRa
  LoRa.beginPacket();
  LoRa.print(dataToSend);
  LoRa.endPacket();

  Serial.println("Data dikirim: " + dataToSend);
  counter++;

  delay(2000);  // Delay antar pengiriman
}
