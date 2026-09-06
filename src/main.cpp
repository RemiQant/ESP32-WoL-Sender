#include <Arduino.h>

#include "secrets.h"
#include "wifi_handler.h"
#include "wol_service.h"
#include "mqtt_handler.h"

WifiHandler wifi(WIFI_SSID, WIFI_PASSWORD);
WolService wol(TARGET_MAC, TARGET_IP, TARGET_PORT);
MqttHandler mqtt(&wol, MQTT_CA_CERT, MQTT_HOST, MQTT_PORT, MQTT_USER, MQTT_PASSWORD, MQTT_TOPIC_COMMAND, MQTT_TOPIC_STATUS);

void setup() {
  Serial.begin(115200);

  wifi.Connect();
  mqtt.Connect();
}

void loop() {
  delay(1000);

  if (!wifi.IsConnected()) {
	Serial.println("[WARNING] Wifi dropped, attempting to reconnect");
	wifi.Connect();
  }

  if (!mqtt.IsConnected()) {
	Serial.println("[WARNING] MQTT Client dropped, attempting to reconnect");
  }
}
