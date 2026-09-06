#pragma once

#include <WiFi.h>

class WifiHandler {
 private:
  const char* ssid_{};
  const char* password_{};

 public:
  WifiHandler(const char* ssid, const char* password);

  void Connect();
  bool IsConnected();
};
