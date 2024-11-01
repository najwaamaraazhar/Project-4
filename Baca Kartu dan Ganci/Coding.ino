#include <SPI.h>
#include <MFRC522.h>


// Definisi pin untuk RFID, LED, Relay, dan Buzzer
#define SS_PIN 10
#define RST_PIN 9
#define ACCESS_DELAY 2000  // Durasi akses diizinkan (2 detik)
#define DENIED_DELAY 1000  // Durasi akses ditolak (1 detik)

MFRC522 mfrc522(SS_PIN, RST_PIN);   

String allowedUIDs[] = {
  "02 6F 86 C4 84 60 00",  // UID pertama yang diizinkan
  "04 0A 3A FA 04 6F 80",  // UID kedua yang diizinkan
};

void setup() {
  // Inisialisasi komunikasi serial dan SPI
  Serial.begin(9600);  
  SPI.begin();          
  mfrc522.PCD_Init();   
  Serial.print("\n");
  Serial.println("Tempatkan kartu di RFID...");

}

void loop() {
  // Cek apakah ada kartu baru
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return; // Jika tidak ada kartu, lanjutkan
  }

  // Pilih kartu
  if (!mfrc522.PICC_ReadCardSerial()) {
    return; // Jika tidak dapat membaca kartu, lanjutkan
  }

  // Menampilkan UID di Serial Monitor dan LCD
  String content = readRFID(); 

  // Cek apakah kartu diizinkan
  if (isAuthorized(content)) {
    grantAccess();  // Fungsi untuk memberi akses
  } else {
    denyAccess();   // Fungsi untuk menolak akses
  }

  // Tampilkan kembali pesan "Siapkan Kartu!" setelah akses diberikan atau ditolak
  delay(2000);
  Serial.print("\n");
  Serial.println("Tempatkan kartu di RFID...");
}

// Membaca UID kartu RFID
String readRFID() {
  String content = "";
  Serial.print("UID tag : ");
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

// Mengecek apakah kartu diizinkan berdasarkan UID
bool isAuthorized(String content) {
  // Loop untuk memeriksa apakah UID ada di daftar yang diizinkan
  for (int i = 0; i < sizeof(allowedUIDs) / sizeof(allowedUIDs[0]); i++) {
    if (content.substring(1) == allowedUIDs[i]) {
      return true;  // Jika cocok, akses diizinkan
    }
  }
  return false;  // Jika tidak cocok, akses ditolak
}

// Fungsi untuk memberikan akses jika kartu diizinkan
void grantAccess() {
  Serial.println("Akses diizinkan");
  Serial.print("Selamat Datang!");
}

// Fungsi untuk menolak akses jika kartu tidak diizinkan
void denyAccess() {
  Serial.println("Akses ditolak");
  Serial.print("Coba Lagi!");
  Serial.print("\n");
 
}
