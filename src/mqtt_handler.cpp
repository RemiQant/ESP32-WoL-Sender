#include "mqtt_handler.h"

#include <Arduino.h>
#include <ESP32MQTTClient.h>

#include <string>

static MqttHandler* g_instance{nullptr};

MqttHandler::MqttHandler(WolService* wol_service, const char* ca_cert, const char* host, const int port, const char* user, const char* password, const char* topic_command, const char* topic_status) 
    : wol_service_{wol_service}, ca_cert_{ca_cert} , host_{host}, port_{port}, user_{user}, password_{password}, topic_command_{topic_command}, topic_status_{topic_status} {
  g_instance = this;
  mqtt_client_.setAutoReconnect(true);
}

void MqttHandler::Connect() {
  mqtt_client_.setURL(host_, port_, user_, password_);
  mqtt_client_.setCaCert(ca_cert_);
  mqtt_client_.loopStart();
}

bool MqttHandler::IsConnected() {
  return mqtt_client_.isConnected();
}

void MqttHandler::OnConnect(esp_mqtt_client_handle_t client) {
  if (mqtt_client_.isMyTurn(client)) {
	mqtt_client_.subscribe(topic_command_, [this](const std::string &payload) {
	  Serial.printf("Received: %s\n", payload.c_str());

	  if (payload == "wake pc") {
		  wol_service_->SendPacket();
		  mqtt_client_.publish(topic_status_, "Wake packet sent", 0, false);
	  }
	});
  }
}

void MqttHandler::OnEvent(esp_mqtt_event_handle_t event) {
  mqtt_client_.onEventCallback(event);
}

void onMqttConnect(esp_mqtt_client_handle_t client) {
  if (g_instance != nullptr) {
	g_instance->OnConnect(client);
  }
}

void handleMQTT(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
  (void)handler_args;
  (void)base;
  (void)event_id;
  auto *event = static_cast<esp_mqtt_event_handle_t>(event_data);
  if (g_instance != nullptr) {
    g_instance->OnEvent(event);
  }
}