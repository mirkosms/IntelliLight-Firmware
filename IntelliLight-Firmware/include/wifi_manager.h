#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

class WiFiManager {
public:
    WiFiManager(const char* ssid, const char* password);
    void connect();
    String getIP();
private:
    const char* ssid;
    const char* password;
};

#endif // WIFI_MANAGER_H
