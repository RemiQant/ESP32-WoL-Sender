#pragma once

#include <WiFiUdp.h>

class WolService {
 private:
  static constexpr int kIpLength{4};
  static constexpr int kMacLength{6};
  static constexpr int kMacRepeats{16};
  static constexpr int kPacketSize{kMacLength + (kMacLength * kMacRepeats)};

  void StrToHex(const char* p, uint8_t* res, int length, int base);

  WiFiUDP udp_{};
  const uint8_t mac_[kMacLength]{};
  const uint8_t ip_[kIpLength]{};
  const uint8_t port_{};

 public:
  WolService(const char* mac, const char* ip, const uint8_t port);

  void SendPacket();
};