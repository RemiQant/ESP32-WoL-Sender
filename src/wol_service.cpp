#include "wol_service.h"

#include <Arduino.h>
#include <WiFiUdp.h>

#include <cstdint>
#include <cstdlib>
#include <cstring>

void WolService::StrToHex(const char* p, uint8_t* res, int length, int base) {
  char* end = nullptr;
  for (int i = 0; i < length; i++) {
    unsigned long str = std::strtoul(p, &end, base);
    if (p == end) { // no more progress can be made
      break;
    }
    res[i] = static_cast<uint8_t>(str);
    if (*end == '\0') { // reached the end of string
      break;
    }
    p = end + 1;    // skips delimiter
  }
}

WolService::WolService(const char* mac, const char* ip_address, uint8_t port)
    : port_{port} {
  StrToHex(mac, mac_, kMacLength, 16);
  StrToHex(ip_address, ip_, kIpLength, 10);
}

void WolService::SendPacket() {
  uint8_t to_send[kPacketSize]{};

  int packet_status{
      udp_.beginPacket(IPAddress(ip_[0], ip_[1], ip_[2], ip_[3]), port_)};

  if (packet_status != 1) {
    Serial.println("[ERROR] Can't establish connection");
    return;
  }

  // Building the Packet
  std::memset(to_send, 0xFF, kMacLength);
  for (int i{1}; i <= kMacRepeats; i++) {
    std::memcpy(&to_send[i * kMacLength], mac_, kMacLength);
  }

  udp_.write(to_send, kPacketSize);

  packet_status = udp_.endPacket();
  if (packet_status != 1) {
    Serial.println("[ERROR] Packet was not sent");
    return;
  }
}