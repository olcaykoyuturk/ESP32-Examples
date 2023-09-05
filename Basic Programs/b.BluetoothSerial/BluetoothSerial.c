#include "BluetoothSerial.h" //Kütüphaneyi projeye dahil etme

BluetoothSerial SerialBT; //BluetoothSerial sınıfından bir nesne oluşturulur. Bu nesne, Bluetooth iletişimi için kullanılacak seri bağlantıyı temsil eder.

void setup() {
  Serial.begin(115200);

  SerialBT.begin(); // Bluetoothu başlatır
  Serial.println("Bluetooth Started! Ready to pair...");
}

void loop() {
  if (Serial.available()) // Seri bağlantıda bekleyen veri olup olmadığını kontrol eder.
  {
    SerialBT.write(Serial.read()); //Bluetoothdan veri gönderme 
  }
  if (SerialBT.available()) // Bluetooth üzerinden bekleyen veri olup olmadığını kontrol eder.
  {
    Serial.write(SerialBT.read()); // Bluetooth üzerinden gelen veriyi alma
  }
  delay(20);
}
