#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Konfigurasi pin LoRa SX1278
#define SS    D8
#define RST   D1
#define DIO0  D0

// LED indikator saat menerima data
#define LED_RX D4

// Konfigurasi WiFi
#define WIFI_SSID     "V2031"
#define WIFI_PASSWORD "11111118"

// Konfigurasi MQTT
#define MQTT_HOST     "test.mosquitto.org"
#define MQTT_PORT     1883
#define MQTT_TOPIC    "lora/warna01"

// Objek MQTT dan WiFi client
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  pinMode(LED_RX, OUTPUT);

  // Koneksi WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi terhubung");

  // Inisialisasi MQTT
  client.setServer(MQTT_HOST, MQTT_PORT);

  // Inisialisasi LoRa
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(433E6)) {
    Serial.println("Gagal memulai LoRa!");
    while (1);
  }
  Serial.println("LoRa siap!");
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String receivedText = "";

    while (LoRa.available()) {
      receivedText += (char)LoRa.read();
    }

    Serial.println("Diterima: " + receivedText);

    // LED berkedip saat data diterima
    digitalWrite(LED_RX, HIGH);
    delay(200);
    digitalWrite(LED_RX, LOW);

    // Parsing CSV "counter,R,G,B"
    int firstComma = receivedText.indexOf(',');
    int secondComma = receivedText.indexOf(',', firstComma + 1);
    int thirdComma = receivedText.indexOf(',', secondComma + 1);

    if (firstComma != -1 && secondComma != -1 && thirdComma != -1) {
      String paketNum = receivedText.substring(0, firstComma);
      String rValue = receivedText.substring(firstComma + 1, secondComma);
      String gValue = receivedText.substring(secondComma + 1, thirdComma);
      String bValue = receivedText.substring(thirdComma + 1);

      // Format JSON untuk dikirim ke Node-RED
      String payload = "{";
      payload += "\"packet\":" + paketNum + ",";
      payload += "\"r\":" + rValue + ",";
      payload += "\"g\":" + gValue + ",";
      payload += "\"b\":" + bValue;
      payload += "}";

      // Kirim ke Node-RED via MQTT
      client.publish(MQTT_TOPIC, payload.c_str());
      Serial.println("Data dikirim ke MQTT:");
      Serial.println(payload);
    } else {
      Serial.println("Format data tidak valid!");
    }

    // Kirim ACK ke transmitter
    delay(100);
    LoRa.beginPacket();
    LoRa.print("ACK");
    LoRa.endPacket();
    Serial.println("ACK dikirim!");

    // LED berkedip 3x
    for (int i = 0; i < 3; i++) {
      digitalWrite(LED_RX, HIGH);
      delay(200);
      digitalWrite(LED_RX, LOW);
      delay(200);
    }
  }
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke MQTT...");
    if (client.connect("LoRaReceiverClient")) {
      Serial.println("Terhubung ke broker");
    } else {
      Serial.print("Gagal, rc=");
      Serial.print(client.state());
      Serial.println(" coba lagi dalam 5 detik");
      delay(5000);
    }
  }
}
