#ifndef __WIFIUSER_H__
#define __WIFIUSER_H__

#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <ESPmDNS.h>   // Used for device hostname (MDNS.begin("esp32"))
#include <esp_wifi.h>  // Used for esp_wifi_restore() to clear stored WiFi credentials

#define FSR_PIN 36     // FSR406B analog pin
#define LED_PIN 2      // WS2812B digtal pin
#define NUM_LEDS 36    // Number of leds on led strips
#define THRESHOLD 1000  // Threshold

extern const char* HOST_NAME;  // Device name
extern int connectTimeOut_s;   // WiFi connection timeout (in seconds)
extern Adafruit_NeoPixel strip;

//=========== Public Functions ===========
void checkConnect(bool reConnect);  // Check if WiFi is connected
void restoreWiFi();                 // Clear stored WiFi credentials
void checkDNS_HTTP();               // Process DNS & HTTP client requests
void connectToWiFi(int timeOut_s);  // Connect to WiFi
void wifiConfig();                  // Set up WiFi configuration mode
void Self_FSR_loop();               // FSR detects pressure and led on/off
void mqttsetup();                   // Integration of MQTT Server setup function
void mqttloop();                    // Integration of MQTT Server loop function
void Home_MQTT_LED();               // Receiving message from MQTT and let on/off
void LEDinit();                     // LED strips initiallization functions

//=========== Internal Functions ===========
void handleRoot();                                               // Handle requests to the root page
void handleConfigWifi();                                         // Handle WiFi configuration form submission
void handleNotFound();                                           // Handle 404 (Not Found) requests
void initSoftAP();                                               // Start AP mode
void initDNS();                                                  // Start DNS server
void initWebServer();                                            // Initialize WebServer
bool scanWiFi();                                                 // Scan nearby WiFi networks for display in the config page
void graduallyLightUp();                                         // led light up when recieve mqtt message
void graduallyTurnOff();                                         // led light off when recieve mqtt message
int getStableFSRValue();                                         // optimize the pressure value reading
void reconnectmqtt();                                            // check if mqtt server is disconnected
void callback(char* topic, byte* payload, unsigned int length);  // callback funtion for mqtt subscribe
void Ledtrunoff();                                               // led turned on when to key placed
void Ledtrunon();                                                // led turned off when to key placed
void WIFILED();                                                  // turn on all leds when wifi connected
void updateLEDAnimation();                                       // led strip turned on animation
void updateLEDTurnOff();                                         // led strip turned off animation

#endif
