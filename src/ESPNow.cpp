#include "ESPNow.h"
#include <cstring>

ESP32NOW::ESP32NOW() {}

ESP32NOW::~ESP32NOW() {}

bool ESP32NOW::init() {
  if (esp_now_init() != ESP_OK) {
    return false;
  }
  
  esp_now_register_recv_cb(onDataReceived);
  esp_now_register_send_cb(onDataSent);
  
  return true;
}

bool ESP32NOW::addPeer(const uint8_t *peerAddr) {
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  memcpy(peerInfo.peer_addr, peerAddr, 6);
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    return false;
  }
  
  return true;
}

bool ESP32NOW::sendData(const uint8_t *data, int len) {
  esp_now_send(NULL, data, len);
  return true;
}

bool ESP32NOW::registerDataReceivedCallback(DataReceivedCallback callback) {
  mDataReceivedCallback = callback;
  return true;
}

bool ESP32NOW::registerDataSentCallback(DataSentCallback callback) {
  mDataSentCallback = callback;
  return true;
}

void ESP32NOW::onDataReceived(const uint8_t *macAddr, const uint8_t *data, int len) {
  if (mDataReceivedCallback != nullptr) {
    mDataReceivedCallback(macAddr, data, len);
  }
}

void ESP32NOW::onDataSent(const uint8_t *macAddr, esp_now_send_status_t status) {
  if (mDataSentCallback != nullptr) {
    mDataSentCallback(macAddr, status);
  }
}

ESP32NOW::DataReceivedCallback ESP32NOW::mDataReceivedCallback = nullptr;
ESP32NOW::DataSentCallback ESP32NOW::mDataSentCallback = nullptr;
