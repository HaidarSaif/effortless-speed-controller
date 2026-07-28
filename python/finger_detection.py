import cv2  # Library untuk pemrosesan citra/video
import time  # Library untuk pengaturan waktu
import os  # Library untuk operasi file dan folder
import HandTrackingModule as htm  # Modul khusus untuk deteksi tangan
import serial  # Library untuk komunikasi serial (dengan hardware)

# Mengatur port serial untuk komunikasi dengan hardware (ESP32)
ser = serial.Serial('COM22', 115200)  # Port 'COM22' dan baudrate 115200 bps
time.sleep(2)  # Tunggu 2 detik agar koneksi serial stabil

# Mengatur resolusi video (lebar dan tinggi)
wCam, hCam = 640, 480

# Mengaktifkan kamera (index 1 untuk kamera kedua, bisa disesuaikan)
cap = cv2.VideoCapture(1)
cap.open(0, cv2.CAP_DSHOW)  # Membuka video capture menggunakan DirectShow (untuk Windows)
cap.set(3, wCam)  # Set lebar frame
cap.set(4, hCam)  # Set tinggi frame

# Membaca gambar overlay dari folder untuk menampilkan jumlah jari
scriptDir = Path(__file__).parent
folderPath = scriptDir / "finger_images"
myList = os.listdir(folderPath)  # Membaca semua file di folder
print(myList)  # Cetak daftar file gambar yang ditemukan

# Menginisialisasi list untuk menyimpan gambar overlay
overlayList = []
for imPath in myList:
    image = cv2.imread(f'{folderPath}/{imPath}')  # Membaca gambar dari file
    overlayList.append(image)  # Menambahkan gambar ke dalam list

print(len(overlayList))  # Cetak jumlah gambar overlay yang dimuat

pTime = 0  # Inisialisasi waktu sebelumnya (untuk menghitung FPS)

# Inisialisasi objek hand detector dengan tingkat kepercayaan
detector = htm.handDetector(detectionCon=1)

# Index titik ujung jari berdasarkan model deteksi tangan (ibu jari, telunjuk, tengah, manis, kelingking)
tipIds = [4, 8, 12, 16, 20]

# Variabel untuk mengatur penundaan pengiriman data serial
last_sent_time = time.time()  # Waktu terakhir data dikirim
delay_time = 0.1  # Delay waktu pengiriman data (100ms)
tempSend = 6  # Variabel untuk menghindari pengiriman data berulang

# Loop utama program
while True:
    success, img = cap.read()  # Membaca frame dari kamera
    if not success:
        continue

    img = cv2.flip(img, 1)  # Membalik gambar secara horizontal (mirror)
    img = detector.findHands(img)  # Deteksi tangan pada frame
    lmList = detector.findPosition(img, draw=False)  # Ambil posisi titik-titik tangan

    if len(lmList) != 0:  # Jika tangan terdeteksi
        fingers = []

        # Logika untuk deteksi ibu jari (thumb)
        if lmList[tipIds[0]][1] > lmList[tipIds[0] - 1][1]:  # Jika ibu jari di sebelah kanan (horizontal)
            fingers.append(1)  # Jari terbuka
        else:
            fingers.append(0)  # Jari tertutup

        # Logika untuk 4 jari lainnya
        for id in range(1, 5):  # Iterasi untuk telunjuk, jari tengah, manis, dan kelingking
            if lmList[tipIds[id]][2] < lmList[tipIds[id] - 2][2]:  # Jika ujung jari lebih tinggi
                fingers.append(1)  # Jari terbuka
            else:
                fingers.append(0)  # Jari tertutup

        totalFingers = fingers.count(1)  # Hitung jumlah jari yang terbuka
        print(totalFingers)  # Cetak jumlah jari yang terbuka ke terminal

        # Mengirim data jumlah jari ke hardware melalui serial jika waktu delay tercapai
        current_time = time.time()
        if current_time - last_sent_time >= delay_time:
            if tempSend != totalFingers:  # Hanya kirim data jika jumlah jari berubah
                ser.write(str(totalFingers).encode())  # Kirim jumlah jari dalam bentuk string
                last_sent_time = current_time  # Perbarui waktu pengiriman terakhir
                tempSend = totalFingers  # Simpan data terakhir yang dikirim

        # Menampilkan gambar overlay sesuai dengan jumlah jari yang terdeteksi
        if 0 < totalFingers <= len(overlayList):
            h, w, c = overlayList[totalFingers - 1].shape
            img[0:h, 0:w] = overlayList[totalFingers - 1]  # Overlay gambar di pojok kiri atas frame

    # Menghitung FPS (Frames Per Second)
    cTime = time.time()
    fps = 1 / (cTime - pTime) if (cTime - pTime) > 0 else 0
    pTime = cTime

    # Menampilkan FPS pada frame (bisa diaktifkan jika perlu)
    # cv2.putText(img, f'FPS: {int(fps)}', (400, 70), cv2.FONT_HERSHEY_PLAIN, 3, (255, 0, 0), 3)

    # Menampilkan frame video
    cv2.imshow("Image", img)
    if cv2.waitKey(1) & 0xFF == ord('q'):  # Tekan 'q' pada keyboard untuk keluar
        break