#include <WiFi.h>

// Pinlerimizi Tanımlıyoruz
int greenLedPin = 27;
int redLedPin = 26;

// Sunucunun portununu tanımlıyoruz. Genellikle 80 portu kullanılır.
WiFiServer server(80);

// Access Pointin ssidsini ve şifresini tanımlıyoruz
const char* ssid = "ESP32-Access-Point";
const char* password = "12345678";

void setup(){
  
  Serial.begin(115200);
  
  // Ledlerimizi output olarak tanımladık
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

  //ACCESS POİNT oluşuruyoruz
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.print("Access Point IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();
}


void loop(){
  
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("START");
    String request = client.readStringUntil('\r');
    client.flush();
    
    // Led on-off komutlarımızı tarayıcıdan alıyoruz
    if (request.indexOf("/green/on") != -1) {
      digitalWrite(greenLedPin, HIGH);
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      client.println("<h1>GREEN LED ON</h1>");
      delay(100);
    }
    
    else if (request.indexOf("/green/off") != -1) {
      digitalWrite(greenLedPin, LOW);
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      client.println("<h1>GREEN LED OFF</h1>");
      delay(100);
    }
    
    else if (request.indexOf("/red/on") != -1) {
      digitalWrite(redLedPin, HIGH);
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      client.println("<h1>RED LED ON</h1>");
      delay(100);
    }
    
    else if (request.indexOf("/red/off") != -1) {
      digitalWrite(redLedPin, LOW);
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      client.println("<h1>RED LED OFF</h1>");
      delay(100);
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<h1>Access Point</h1>");
    client.println("<p><a href=\"/green/on\">GREEN LED ON</a></p>");
    client.println("<p><a href=\"/green/off\">GREEN LED OFF</a></p>");
    client.println("<p><a href=\"/red/on\">RED LED ON</a></p>");
    client.println("<p><a href=\"/red/off\">RED LED OFF</a></p>");
    client.println("</html>");
    delay(100);
  }
}
