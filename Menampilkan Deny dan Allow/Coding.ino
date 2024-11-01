#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
int sda = A4;
int scl = A5;
// Pin dan konfigurasi RFID
#define RST_PIN         9  // Pin RST RFID
#define SS_PIN          10 // Pin SDA RFID

MFRC522 rfid(SS_PIN, RST_PIN);  // Membuat instance RFID
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Inisialisasi LCD I2C dengan alamat 0x27

// Pin LED
const int ledPin = 3;   // Pin LED (sama untuk deny dan allow)

// UID yang diizinkan (allow) dengan panjang 7 byte
byte allowUIDs[][7] = {
  {0x05, 0x89, 0xB9, 0x35, 0x02, 0x32, 0x00}  // Ganti dengan UID KTP Anda
};

// UID yang ditolak (deny) dengan panjang 4 byte
byte denyUIDs[][4] = {
  {0x23, 0xB8, 0xAE, 0xFE}  // Ganti dengan UID kartu yang ditolak
};

void setup() {
  Serial.begin(9600);   // Memulai komunikasi serial
  SPI.begin();          // Memulai SPI bus
  rfid.PCD_Init();      // Inisialisasi modul RFID
  
  lcd.init();           // Inisialisasi LCD
  lcd.backlight();      // Mengaktifkan lampu latar LCD
  lcd.clear();

  pinMode(ledPin, OUTPUT);   // Atur pin LED sebagai output

  Serial.println("Letakkan tag RFID di dekat modul...");
  lcd.setCursor(0, 0);
  lcd.print("Scan your card");
}

void loop() {
  // Jika tidak ada kartu yang terdeteksi, keluar dari loop
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Jika tidak bisa membaca kartu, keluar dari loop
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Cek apakah tag cocok dengan UID yang diizinkan
  if (checkUID7(rfid.uid.uidByte, rfid.uid.size, allowUIDs, sizeof(allowUIDs) / 7)) {
    allowAccess();  // Jika cocok, akses diizinkan
  } 
  // Cek apakah tag cocok dengan UID yang ditolak
  else if (checkUID4(rfid.uid.uidByte, rfid.uid.size, denyUIDs, sizeof(denyUIDs) / 4)) {
    denyAccess();   // Jika cocok dengan UID yang ditolak, akses ditolak
  } 
  else {
    denyAccess();   // Jika tidak cocok dengan keduanya, akses tetap ditolak
  }

  // Berhenti membaca kartu
  rfid.PICC_HaltA();
}

// Fungsi untuk memeriksa apakah UID dari tag dengan panjang 7 byte cocok dengan daftar UID tertentu
bool checkUID7(byte *tagUID, byte tagSize, byte allowedUIDs[][7], int numUIDs) {
  if (tagSize != 7) {
    return false;
  }
  for (int i = 0; i < numUIDs; i++) {
    bool match = true;
    for (byte j = 0; j < 7; j++) {
      if (tagUID[j] != allowedUIDs[i][j]) {
        match = false;
        break;
      }
    }
    if (match) {
      return true;  // Jika UID cocok, kembalikan true
    }
  }
  return false;  // Jika tidak ada yang cocok, kembalikan false
}

// Fungsi untuk memeriksa apakah UID dari tag dengan panjang 4 byte cocok dengan daftar UID tertentu
bool checkUID4(byte *tagUID, byte tagSize, byte allowedUIDs[][4], int numUIDs) {
  if (tagSize != 4) {
    return false;
  }
  for (int i = 0; i < numUIDs; i++) {
    bool match = true;
    for (byte j = 0; j < 4; j++) {
      if (tagUID[j] != allowedUIDs[i][j]) {
        match = false;
        break;
      }
    }
    if (match) {
      return true;  // Jika UID cocok, kembalikan true
    }
  }
  return false;  // Jika tidak ada yang cocok, kembalikan false
}

// Fungsi untuk memberikan akses (Allow)
void allowAccess() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Access: ALLOW");
  Serial.println("Akses diizinkan");

  digitalWrite(ledPin, HIGH);  // Nyalakan LED terus
  delay(3000);                 // LED menyala selama 3 detik
  digitalWrite(ledPin, LOW);   // Matikan LED

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan your card");
}

// Fungsi untuk menolak akses (Deny)
void denyAccess() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Access: DENY");
  Serial.println("Akses ditolak");

  for (int i = 0; i < 6; i++) {  // Blink LED selama 3 detik
    digitalWrite(ledPin, HIGH);
    delay(250);
    digitalWrite(ledPin, LOW);
    delay(250);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan your card");
}
