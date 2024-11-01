#include <SPI.h>
#include <MFRC522.h>

// Definisi pin untuk RFID
#define SS_PIN 10
#define RST_PIN 9
#define ACCESS_DELAY 2000  // Durasi akses diizinkan (2 detik)
#define DENIED_DELAY 1000  // Durasi akses ditolak (1 detik)

MFRC522 mfrc522(SS_PIN, RST_PIN);    // Inisialisasi RFID reader

// Daftar UID yang diizinkan (contoh UID dari KTP yang valid)
String allowedUIDs[] = {
  "05 87 3B CE F1 B2 00",  // UID KTP pertama yang diizinkan
    // UID KTP kedua yang diizinkan
};

void setup() {
  // Inisialisasi komunikasi serial dan SPI
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();   // Inisialisasi RFID reader
  Serial.println("Tempatkan KTP di RFID...");
}

void loop() {
  // Cek apakah ada kartu baru (KTP)
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return; // Jika tidak ada kartu, lanjutkan
  }

  // Pilih kartu
  if (!mfrc522.PICC_ReadCardSerial()) {
    return; // Jika tidak dapat membaca kartu, lanjutkan
  }

  // Membaca UID dari kartu (KTP) dan menampilkan di Serial Monitor
  String content = readRFID();

  // Cek apakah kartu (KTP) diizinkan
  if (isAuthorized(content)) {
    grantAccess();  // Fungsi untuk memberi akses
  } else {
    denyAccess();   // Fungsi untuk menolak akses
  }

  delay(2000);  // Delay untuk menunggu kartu berikutnya
  Serial.println("Tempatkan KTP di RFID...");
}

// Membaca UID kartu (KTP)
String readRFID() {
  String content = "";
  Serial.print("UID KTP: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  content.toUpperCase();  // Ubah UID menjadi huruf besar
  return content;
}

// Mengecek apakah KTP diizinkan berdasarkan UID
bool isAuthorized(String content) {
  // Loop untuk memeriksa apakah UID ada di daftar yang diizinkan
  for (int i = 0; i < sizeof(allowedUIDs) / sizeof(allowedUIDs[0]); i++) {
    if (content.substring(1) == allowedUIDs[i]) {
      return true;  // Jika cocok, akses diizinkan
    }
  }
  return false;  // Jika tidak cocok, akses ditolak
}

// Fungsi untuk memberikan akses jika KTP diizinkan
void grantAccess() {
  Serial.println("Akses diizinkan");
  Serial.println("Selamat Datang!");
}

// Fungsi untuk menolak akses jika KTP tidak diizinkan
void denyAccess() {
  Serial.println("Akses ditolak");
  Serial.println("Coba Lagi!");
}
