#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

// Replace with your WiFi credentials
const char* ssid = "Moss";
const char* password = "unclerico67";

// LED Pin (GPIO2 is built-in LED on most ESP32 dev boards)
const int LED_PIN = 5; // Use 2 

// Web server on port 80
WebServer server(80);

// JSON document for responses
DynamicJsonDocument doc(256);

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
  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());
}

// Send JSON response helper
void sendJson(int statusCode, const JsonDocument& json) {
  String response;
  serializeJson(json, response);
  server.send(statusCode, "application/json", response);
}

// Root endpoint - status
void handleRoot() {
  doc.clear();
  doc["status"] = "ok";
  doc["led"] = digitalRead(LED_PIN) ? "on" : "off";
  doc["uptime"] = millis() / 1000;
  doc["ip"] = WiFi.localIP().toString();

  sendJson(200, doc);
}

// Turn LED ON
void handleLedOn() {
  digitalWrite(LED_PIN, HIGH);
  doc.clear();
  doc["led"] = "on";
  doc["message"] = "LED turned ON";
  sendJson(200, doc);
}

// Turn LED OFF
void handleLedOff() {
  digitalWrite(LED_PIN, LOW);
  doc.clear();
  doc["led"] = "off";
  doc["message"] = "LED turned OFF";
  sendJson(200, doc);
}

// POST /led with JSON body
void handleLedPost() {
  if (!server.hasArg("plain")) {
    doc.clear();
    doc["error"] = "No body sent";
    sendJson(400, doc);
    return;
  }

  String body = server.arg("plain");
  DeserializationError error = deserializeJson(doc, body);

  if (error) {
    doc.clear();
    doc["error"] = "Invalid JSON";
    doc["details"] = error.c_str();
    sendJson(400, doc);
    return;
  }

  if (!doc.containsKey("state")) {
    doc.clear();
    doc["error"] = "Missing 'state' field. Use 'on' or 'off'";
    sendJson(400, doc);
    return;
  }

  String state = doc["state"].as<String>();
  state.toLowerCase();

  if (state == "on") {
    digitalWrite(LED_PIN, HIGH);
    doc.clear();
    doc["led"] = "on";
    doc["message"] = "LED turned ON via POST";
  } else if (state == "off") {
    digitalWrite(LED_PIN, LOW);
    doc.clear();
    doc["led"] = "off";
    doc["message"] = "LED turned OFF via POST";
  } else {
    doc.clear();
    doc["error"] = "Invalid state. Use 'on' or 'off'";
    sendJson(400, doc);
    return;
  }

  sendJson(200, doc);
}

// 404 handler
void handleNotFound() {
  doc.clear();
  doc["error"] = "Not found";
  doc["path"] = server.uri();
  sendJson(404, doc);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Initialize LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // WiFi setup
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  unsigned long startAttemptTime = millis();
  const unsigned long timeout = 20000;

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout) {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFailed to connect to WiFi.");
  } else {
    Serial.println("\nConnected to WiFi!");
    printWifiInfo();

    // === REST API Routes ===
    server.on("/", HTTP_GET, handleRoot);
    server.on("/led/on", HTTP_GET, handleLedOn);
    server.on("/led/off", HTTP_GET, handleLedOff);
    server.on("/led", HTTP_POST, handleLedPost);

    // Enable CORS (optional, for browser access)
    server.enableCORS(true);

    // 404
    server.onNotFound(handleNotFound);

    // Start server
    server.begin();
    Serial.println("HTTP server started");
    Serial.println("API Ready! Use:");
    Serial.println("  GET  http://" + WiFi.localIP().toString() + "/");
    Serial.println("  GET  http://" + WiFi.localIP().toString() + "/led/on");
    Serial.println("  GET  http://" + WiFi.localIP().toString() + "/led/off");
    Serial.println("  POST http://" + WiFi.localIP().toString() + "/led  + JSON: {\"state\":\"on\"}");
  }
}

void loop() {
  // Handle client requests
  server.handleClient();

  // Reconnect WiFi if lost
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
      server.begin(); // Restart server
    }
  }

  delay(10); // Allow CPU to breathe
}