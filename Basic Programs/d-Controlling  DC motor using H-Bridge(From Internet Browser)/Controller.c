#include <WiFi.h>

// Pin tanımlamaları
#define PIN_IN1  27
#define PIN_IN2  26
#define PIN_ENA  14

WiFiServer server(80);

// WiFi ağı için SSID ve şifre
const char* ssid = "ESP32-Access-Point";
const char* password = "12345678";

void setup() {
  
  // Çıkış pinlerini ayarlama
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENA, OUTPUT);

  Serial.begin(115200);

  // Erişim noktası (Access Point) oluşturma
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.print("Access Point IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    if (client.available()) {
      Serial.println("START");
      // İstek alma
      String request = client.readStringUntil('\r');
      client.flush();

      // İstekte "/speed" ifadesi var mı yok mu kontrol etme
      if (request.indexOf("/speed") != -1) {
        // Hız değeri alma "value="
        int speedIndex = request.indexOf("value=");
        if (speedIndex != -1) {
          int speedValue = request.substring(speedIndex + 6).toInt();
          // Motor kontrolü
          digitalWrite(PIN_IN1, HIGH);
          digitalWrite(PIN_IN2, LOW);
          analogWrite(PIN_ENA, speedValue);
          // Yanıt gönderme
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<h1>SPEED: </h1>");
        }
      }
    }
    delay(10);
    client.stop();
  }
}

