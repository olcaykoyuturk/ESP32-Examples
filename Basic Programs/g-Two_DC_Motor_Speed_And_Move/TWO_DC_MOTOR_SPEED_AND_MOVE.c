#include <WiFi.h>

#define PIN_IN1 27
#define PIN_IN2 26
#define PIN_ENA 14
#define PIN_IN3 18
#define PIN_IN4 19
#define PIN_ENB 5

WiFiServer server(80);

const char* ssid = "ESP32-Access-Point";
const char* password = "12345678";

void setup() {

  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);

  Serial.begin(115200);
  
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
      String request = client.readStringUntil('\r');
      client.flush();

      if (request.indexOf("/speed") != -1) {
        int speedIndex = request.indexOf("value=");
        if (speedIndex != -1) {
          int speedValue = request.substring(speedIndex + 6).toInt();
          speed(speedValue);        // 192.168.4.1/speed?value=255
          sendResponse(client, "Speed");
        }
      }
      
      else if (request.indexOf("/command") != -1) {

        if (request.indexOf("?command=forward") != -1) {
          moveForward();            // 192.168.4.1/command?command=forward
          sendResponse(client, "Moving forward"); 
        } 
        else if (request.indexOf("?command=backward") != -1) { 
          moveBackward();
          sendResponse(client, "Moving backward");
        }
        else if (request.indexOf("?command=stop") != -1){
          moveStop();
          sendResponse(client, "Stop");
        }
      }
    }
  }
}

void moveForward() {
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, HIGH);
}

void moveBackward() {
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, HIGH);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, HIGH);
}

void moveStop(){
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
}

void speed(int speedValue2){
  analogWrite(PIN_ENA, speedValue2);
  analogWrite(PIN_ENB, speedValue2);
}

void sendResponse(WiFiClient client, String message) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println("<h1>" + message + "</h1>");
}
