# Node-RED-LoRaSX1278-Telemetry-

Proyek ini merupakan sistem telemetri berbasis **ESP8266 dan LoRa SX1278** untuk mengirimkan data sensor secara nirkabel dan divisualisasikan menggunakan **Node-RED dashboard**.

Fitur
- Transmisi data sensor menggunakan **LoRa SX1278**
- Parsing data dengan **JSON format** di Node-RED
- Visualisasi data real-time melalui **Node-RED Dashboard**
- Dokumentasi pengujian perangkat keras dan integrasi sistem

Teknologi
- **Hardware**: ESP8266, LoRa SX1278, Sensor Warna (TCS3200/TCS34725)
- **Software**: Arduino IDE, Node-RED, MQTT (optional)

Struktur Repo
- `Program_JSON_Node-RED/` → Flow Node-RED (export file JSON)
- `Program_sensor_warna/` → Source code Arduino untuk membaca sensor warna
- `Wairing dan cara penggunaan/` → Gambar wiring, dan petunjuk penggunan

#Dokumentasi
Wiring Diagram

Pemancar
<img width="532" height="457" alt="image" src="https://github.com/user-attachments/assets/956b215b-21ad-4a31-b12f-82ca427e7105" />

Penerima
<img width="462" height="382" alt="image" src="https://github.com/user-attachments/assets/0b0c3ce6-7981-4eb4-ad19-a6612cec3f13" />

### Dashboard Node-RED
<img width="915" height="486" alt="image" src="https://github.com/user-attachments/assets/cd2a23f9-fa5f-49c0-853b-f4a40ac2d352" />

### Video Demo & Buku Pnduan
https://drive.google.com/drive/folders/1hWWEPuip4RN4peL4tBfAAkmqG0LhBO7n?usp=sharing.

Cara Menjalankan
1. Upload kode pada folder `Program_sensor_warna` ke **ESP8266** melalui Arduino IDE.
2. Import flow di folder `Program_JSON_Node-RED` ke aplikasi **Node-RED**.
3. Jalankan Node-RED dan buka `localhost:1880/ui` untuk melihat dashboard.
4. Hubungkan perangkat LoRa SX1278 sesuai wiring.

Author
**Nauval Hibrizi**  
IoT & Embedded Systems Enthusiast | Mechatronics Engineering  
[LinkedIn](https://linkedin.com/in/nauvalhibrizi) | [Email](mailto:nauvalhibrizi61@gmail.com)
