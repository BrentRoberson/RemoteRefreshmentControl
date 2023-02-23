#ifndef ESP32NOW_H
#define ESP32NOW_H

#include <esp_now.h>

class ESP32NOW {
  public:
    typedef void (*DataReceivedCallback)(const uint8_t *macAddr, const uint8_t *data, int len);
    typedef void (*DataSentCallback)(const uint8_t *macAddr, esp_now_send_status_t status);
    
    ESP32NOW();
    ~ESP32NOW();
    
    bool init();
    bool addPeer(const uint8_t *peerAddr);
    bool sendData(const uint8_t *data, int len);
    bool registerDataReceivedCallback(DataReceivedCallback callback);
    bool registerDataSentCallback(DataSentCallback callback);
    
  private:
    static void onDataReceived(const uint8_t *macAddr, const uint8_t *data, int len);
    static void onDataSent(const uint8_t *macAddr, esp_now_send_status_t status);
    
    static DataReceivedCallback mDataReceivedCallback;
    static DataSentCallback mDataSentCallback;
};


#endif // ESP32NOW_H
