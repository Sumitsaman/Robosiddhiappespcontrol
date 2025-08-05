#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>  // ✅ ESP32-compatible servo library

// WiFi credentials
const char* ssid = "sumit";
const char* password = "12345678";

// Static IP settings
IPAddress local_IP(192, 168, 137, 85);     // ✅ Pick a unique and aesthetic IP
IPAddress gateway(192, 168, 137, 1);        // Your router's IP
IPAddress subnet(255, 255, 255, 0);         // Subnet mask

// Web server on port 80
WebServer server(80);

// Pin assignments
const int ledPin = 2;
const int servoPin = 5;
const int motorPin1 = 18;
const int motorPin2 = 19;

Servo myServo;

void setup() {
  Serial.begin(115200);

  // Set pin modes
  pinMode(ledPin, OUTPUT);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);

  myServo.attach(servoPin);

  // Apply static IP before WiFi.begin
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("❌ Failed to configure Static IP");
  }

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("✅ Connected! IP Address: ");
  Serial.println(WiFi.localIP());

  // LED endpoints
  server.on("/led/on", []() {
    digitalWrite(ledPin, HIGH);
    server.send(200, "text/plain", "LED ON");
  });

  server.on("/led/off", []() {
    digitalWrite(ledPin, LOW);
    server.send(200, "text/plain", "LED OFF");
  });

  // Servo endpoints
  server.on("/servo/left", []() {
    myServo.write(0);
    server.send(200, "text/plain", "Servo Left");
  });

  server.on("/servo/center", []() {
    myServo.write(90);
    server.send(200, "text/plain", "Servo Center");
  });

  server.on("/servo/right", []() {
    myServo.write(180);
    server.send(200, "text/plain", "Servo Right");
  });

  // DC motor endpoints
  server.on("/motor/on", []() {
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    server.send(200, "text/plain", "Motor ON");
  });

  server.on("/motor/off", []() {
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
    server.send(200, "text/plain", "Motor OFF");
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
