#ifndef __WIFIUSER_H__
#define __WIFIUSER_H__
 
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <ESPmDNS.h>      // Used for device hostname (MDNS.begin("esp32"))
#include <esp_wifi.h>     // Used for esp_wifi_restore() to clear stored WiFi credentials
 
extern const char* HOST_NAME;       // Device name
extern int connectTimeOut_s;        // WiFi connection timeout (in seconds)

//=========== Public Functions ===========
void checkConnect(bool reConnect);    // Check if WiFi is connected
void restoreWiFi();                   // Clear stored WiFi credentials
void checkDNS_HTTP();                 // Process DNS & HTTP client requests
void connectToWiFi(int timeOut_s);    // Connect to WiFi

//=========== Internal Functions ===========
void handleRoot();                    // Handle requests to the root page
void handleConfigWifi();               // Handle WiFi configuration form submission
void handleNotFound();                 // Handle 404 (Not Found) requests
void initSoftAP();                     // Start AP mode
void initDNS();                        // Start DNS server
void initWebServer();                  // Initialize WebServer
bool scanWiFi();                       // Scan nearby WiFi networks for display in the config page
void wifiConfig();                     // Set up WiFi configuration mode

#endif
