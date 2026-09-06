#include "wifi_handler.h"

#include <Arduino.h>
#include <WiFi.h>

WifiHandler::WifiHandler(const char* ssid, const char* password)
    : ssid_{ssid}, password_{password} {
  WiFi.setAutoReconnect(true);
}

void WifiHandler::Connect() {
  Serial.printf("Connecting to %s\n", ssid_);

  WiFi.begin(ssid_, password_);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    if (WiFi.status() == WL_CONNECT_FAILED) {
      Serial.println("\n[ERROR] Connection failed, please check WiFi credentials");
      return;
    }
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

bool WifiHandler::IsConnected() { 
  return WiFi.status() == WL_CONNECTED; 
}
