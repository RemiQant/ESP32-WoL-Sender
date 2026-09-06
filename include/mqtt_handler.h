#pragma once

#include <ESP32MQTTClient.h>

#include "wol_service.h"

class MqttHandler {
 private:
  WolService* wol_service_{};
	ESP32MQTTClient mqtt_client_{};
  const char* ca_cert_{};
  const char* host_{};
  const char* user_{};
  const char* password_{};
  const char* topic_command_{};
  const char* topic_status_{};
  const int port_{};

 public:
  MqttHandler(WolService* wol_service, const char* ca_cert, const char* host, const int port, const char* user, const char* password,  const char* topic_command, const char* topic_status);

  void Connect();
  bool IsConnected();
  void OnConnect(esp_mqtt_client_handle_t client);
  void OnEvent(esp_mqtt_event_handle_t event);
};