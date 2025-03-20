#include "WiFiUser.h"


const int resetPin = 0;     // Reset button pin, used to clear WiFi credentials
int connectTimeOut_s = 15;  // WiFi connection timeout (seconds)

void setup() {
  pinMode(resetPin, INPUT_PULLUP);  // Set button as input with pull-up (default HIGH, pulled LOW when pressed)
  
  Serial.begin(115200);             // Set baud rate
  connectToWiFi(connectTimeOut_s);
  mqttsetup();
  LEDinit();
}

void loop() {
  if (!digitalRead(resetPin)) // Press and hold for 5 seconds to clear WiFi credentials
  {
    delay(5000);  // This method is not precise
    if (!digitalRead(resetPin))
    {
      Serial.println("\nButton held for 5 seconds, clearing saved WiFi credentials.");
      restoreWiFi();  // Erase stored WiFi credentials
      ESP.restart();  // Restart ESP32
      Serial.println("Device restarting..."); // This might not be printed before the restart
    }
  }

  checkDNS_HTTP();     // Process DNS & HTTP client requests (for captive portal)
  checkConnect(true);  // Check WiFi connection status, reconnect if disconnected and integret mqtt connection functions, mearsuring sensor value functions and MQTT message pulish functions.
}
