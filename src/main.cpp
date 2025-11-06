#include <Arduino.h>
#include <WiFi.h>

// Replace with your WiFi credentials
const char* ssid = "Moss";
const char* password = "unclerico67";

void printWifiInfo() {
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway IP: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("DNS: ");
  Serial.println(WiFi.dnsIP());
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("RSSI (signal strength): ");
  Serial.println(WiFi.RSSI());
}

void setup() {
  Serial.begin(115200); // 115200
  delay(1000);  // Give Serial time to initialize

  // Set WiFi to station mode
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  
  // Wait for connection with timeout and dots for feedback
  unsigned long startAttemptTime = millis();
  const unsigned long timeout = 20000;  // 20 seconds timeout

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout) {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFailed to connect to WiFi. Check credentials or signal.");
    // Optional: restart ESP32
    // ESP.restart();
  } else {
    Serial.println("\nConnected to WiFi!");
    printWifiInfo();
  }
}

void loop() {
  // Reconnect if connection is lost
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected. Reconnecting...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    
    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
      delay(500);
      Serial.print(".");
    }
    
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nReconnected!");
      printWifiInfo();
    }
  }
  
  delay(10000);  // Check every 10 seconds
}
