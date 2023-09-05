#include <WiFi.h>

#define PIN_IN1  27     // Motor sürücüsünün IN1 pini
#define PIN_IN2  26     // Motor sürücüsünün IN2 pini
#define PIN_ENA  14     // Motor sürücüsünün ENA pini

WiFiServer server(80);  // Portu 80 olarak ayarlar. Genellikle 80 kullanılır

const char* ssid = "ESP32-Access-Point";   // Erişim noktası (Access Point) adı
const char* password = "12345678";          // Erişim noktası şifresi

void setup() {
  pinMode(PIN_IN1, OUTPUT);   // IN1 pini çıkış olarak ayarlanır
  pinMode(PIN_IN2, OUTPUT);   // IN2 pini çıkış olarak ayarlanır
  pinMode(PIN_ENA, OUTPUT);   // ENA pini çıkış olarak ayarlanır

  Serial.begin(115200);       // Haberleşme hızı ayarlanır

  WiFi.mode(WIFI_AP);         // WiFi modu erişim noktası (Access Point) olarak ayarlanır
  WiFi.softAP(ssid, password);   // Erişim noktası oluşturulur
  Serial.print("Access Point IP address: ");
  Serial.println(WiFi.softAPIP());   // Erişim noktasının IP adresi seri porta yazdırılır
  server.begin();   // WiFi sunucusu başlatılır
}

void loop() {
  WiFiClient client = server.available();   // Gelen istemci bağlantısını kabul eder

  if (client) {
    if (client.available()) {
      Serial.println("START");

      String request = client.readStringUntil('\r');   // İstemciden gelen isteği okur
      client.flush();

      if (request.indexOf("/command") != -1) {   // İstek "/command" yolunu içeriyorsa

        if (request.indexOf("?command=forward") != -1) {
          moveForward();   // İleri hareket komutunu çağırır
          sendResponse(client, "Moving forward");
        } 
        else if (request.indexOf("?command=backward") != -1) {
          moveBackward();   // Geri hareket komutunu çağırır
          sendResponse(client, "Moving backward");
        }
        else if (request.indexOf("?command=stop") != -1){
          moveStop();   // Durma komutunu çağırır
          sendResponse(client, "Moving stop");
        }
      }
    }
  }
}

void moveForward() {
  digitalWrite(PIN_IN1, LOW);    // IN1 pini LOW (düşük) seviyesine ayarlanır
  digitalWrite(PIN_IN2, HIGH);   // IN2 pini HIGH (yüksek) seviyesine ayarlanır
  digitalWrite(PIN_ENA, HIGH);   // ENA pini HIGH (yüksek) seviyesine ayarlanır
}

void moveBackward() {
  digitalWrite(PIN_IN1, HIGH);   // IN1 pini HIGH (yüksek) seviyesine ayarlanır
  digitalWrite(PIN_IN2, LOW);    // IN2 pini LOW (düşük) seviyesine ayarlanır
  digitalWrite(PIN_ENA, HIGH);   // ENA pini HIGH (yüksek) seviyesine ayarlanır
}

void moveStop(){
  digitalWrite(PIN_IN1, LOW);   // IN1 pini HIGH (yüksek) seviyesine ayarlanır
  digitalWrite(PIN_IN2, LOW);   // IN2 pini HIGH (yüksek) seviyesine ayarlanır
  digitalWrite(PIN_ENA, LOW);    // ENA pini LOW (düşük) seviyesine ayarlanır
}

void sendResponse(WiFiClient client, String message) {
  client.println("HTTP/1.1 200 OK");   // İstemciye başarılı yanıt başlığını gönderir
  client.println("Content-Type: text/html");   // İçerik türünü belirtir
  client.println();   // Boş satır
  client.println("<h1>" + message + "</h1>");   // İstemciye HTML yanıtı gönderir
}

