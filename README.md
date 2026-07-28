# 🖐️ Effortless Speed Controller (ESC)

## 📖 Deskripsi Proyek
Effortless Speed Controller adalah sistem kendali perangkat elektronik berbasis pengenalan gestur (*Gesture Recognition*). Proyek mandiri ini dirancang untuk mempermudah pengguna dalam menghidupkan, mematikan, serta mengatur intensitas/kecepatan alat elektronik di sekitar, seperti kipas angin atau AC ruangan.

Alat ini menggunakan modul kamera yang terhubung ke Raspberry Pi untuk membaca gestur jari tangan manusia melalui algoritma *Computer Vision*. Data perintah kemudian dikirimkan secara nirkabel untuk menggerakkan aktuator relai pada perangkat.

**Fitur Utama:**
*   Pengenalan gestur jari (*Hand Tracking*) secara *real-time*.
*   Kendali ON/OFF dan kecepatan perangkat elektronik tanpa sentuhan fisik.
*   Komunikasi nirkabel antar mikrokontroler menggunakan protokol ESP-NOW dengan latensi rendah.

## 🚀 Teknologi yang Digunakan
*   **Hardware:** Raspberry Pi, ESP32, Modul Kamera, Modul Relay.
*   **Software:** Python, OpenCV (Computer Vision), C++, Protokol ESP-NOW.

## 📂 Struktur Direktori
*   `/computer_vision` - Berisi *script* Python dan model OpenCV untuk deteksi gestur di Raspberry Pi.
*   `/microcontroller` - Berisi *source code* ESP32 (C++) sebagai penerima perintah aktuator.
*   `/documentation` - Berisi *laporan proyek.
