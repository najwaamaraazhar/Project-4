#include <DHT.h>

#define DHTPIN 2     // Pin yang terhubung ke sensor DHT
#define DHTTYPE DHT22   // Ganti dengan DHT11 jika menggunakan sensor DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();      // Membaca kelembaban
  float temperature = dht.readTemperature(); // Membaca suhu dalam Celcius

  // Jika gagal membaca sensor
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Menampilkan hasil ke Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");

  delay(2000); // Tunggu 2 detik sebelum pembacaan berikutnya
}
