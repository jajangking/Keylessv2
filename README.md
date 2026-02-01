# Keylessv2

Sistem Keyless untuk Kendaraan Bermotor berbasis ESP32

## Deskripsi Fungsi:
- Single Press tombol: Toggle kontak ON/OFF (bisa digunakan tanpa koneksi)
- Double Press tombol: Aktifkan starter (jika kontak menyala, bisa digunakan tanpa koneksi)
- Web Interface: Kontrol penuh melalui browser
- Bluetooth: Alternatif koneksi jika WiFi tidak tersedia

## Komponen:
- 8 LED untuk indikasi status
- 1 Tombol untuk kontrol manual
- 1 Buzzer untuk notifikasi suara
- 2 Relay untuk kontrol kontak dan starter

## Konfigurasi GPIO:
- LED1 (Ready) → GPIO 4
- LED2 (BT Connected) → GPIO 5
- LED3 (WiFi Connected) → GPIO 18
- LED4 (Kontak ON) → GPIO 19
- LED5 (Mesin ON) → GPIO 21
- LED6 (Starter LED) → GPIO 22
- LED7 (Disconnect Alert) → GPIO 23
- LED8 (Alarm) → GPIO 25
- Button → GPIO 13
- Buzzer → GPIO 15
- Relay CH1 (Kontak - ON/OFF Switch) → GPIO 26
- Relay CH2 (Starter - Momentary Switch) → GPIO 27

## Troubleshooting:
- Jika ESP32 tidak merespons, pastikan kabel USB terhubung dengan baik
- Sistem akan tetap berfungsi meskipun tidak terhubung ke WiFi/Bluetooth
- Kontrol melalui tombol fisik tetap berfungsi tanpa koneksi jaringan
- Jika tidak ada koneksi jaringan, LED WiFi dan BT akan mati
- Jika sistem aktif (kontak/mesin menyala) tapi tidak terhubung, alarm akan aktif
