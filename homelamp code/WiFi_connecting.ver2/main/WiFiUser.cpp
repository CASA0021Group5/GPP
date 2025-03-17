#include "WiFiUser.h"
 
const byte DNS_PORT = 53;                  // Set DNS port
const int webPort = 80;                    // Set Web port
 
const char* AP_SSID  = "ESP32-4_1";        // Set AP SSID
//const char* AP_PASS  = "";               // No password for AP
 
const char* HOST_NAME = "MY_ESP32";        // Set device name
String scanNetworksID = "";                // Store scanned WiFi SSIDs
 
IPAddress apIP(192, 168, 4, 1);            // Set AP IP address
 
String wifi_ssid = "";                     // Temporarily store WiFi SSID
String wifi_pass = "";                     // Temporarily store WiFi password
 
DNSServer dnsServer;                       // Create DNS server instance
WebServer server(webPort);                 // Start web server
 
#define ROOT_HTML  "<!DOCTYPE html><html><head><title>WIFI</title><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><style type=\"text/css\">.input{display: block; margin-top: 10px;}.input span{width: 100px; float: left; float: left; height: 36px; line-height: 36px;}.input input{height: 30px;width: 200px;}.btn{width: 120px; height: 35px; background-color: #000000; border:0px; color:#ffffff; margin-top:15px; margin-left:100px;}</style><body><form method=\"POST\" action=\"configwifi\"><label class=\"input\"><span>WiFi SSID</span><input type=\"text\" name=\"ssid\" value=\"\"></label><label class=\"input\"><span>WiFi PASS</span> <input type=\"text\"  name=\"pass\"></label><input class=\"btn\" type=\"submit\" name=\"submit\" value=\"Submit\"> <p><span> Nearby WiFi:</P></form>"

/*
 * Handle root URL request
 */
void handleRoot() {
  server.send(200, "text/html", ROOT_HTML + scanNetworksID + "</body></html>");
}

/*
 * Handle WiFi configuration submission
 */
void handleConfigWifi() {
    if (server.hasArg("ssid") && server.hasArg("pass")) {
        wifi_ssid = server.arg("ssid");
        wifi_pass = server.arg("pass");

        Serial.println("Received WiFi Configuration:");
        Serial.println("SSID: " + wifi_ssid);
        Serial.println("Password: " + wifi_pass);

        server.send(200, "text/html", "<meta charset='UTF-8'>WiFi information received. Attempting to connect...");

        delay(2000);
        WiFi.softAPdisconnect(true);  // Disable AP
        server.close();               // Stop web server

        connectToWiFi(30);  // Connect to WiFi with a timeout of 30 seconds
    } else {
        server.send(200, "text/html", "<meta charset='UTF-8'>Incomplete WiFi information provided!");
    }
}

/*
 * Handle requests for non-existent resources
 */
void handleNotFound() {           
  handleRoot();  // Redirect to WiFi configuration page
}

/*
 * Start AP mode
 */
void initSoftAP() {
  WiFi.mode(WIFI_AP);                                        
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // Set AP IP and subnet mask
  if (WiFi.softAP(AP_SSID)) {                           
    Serial.println("ESP-32S SoftAP started.");
    Serial.print("Soft-AP IP address = ");
    Serial.println(WiFi.softAPIP());                          
    Serial.println(String("MAC address = ") + WiFi.softAPmacAddress().c_str());
  } else { 
    Serial.println("WiFi AP setup failed");
    delay(1000);
    Serial.println("Restarting...");
    ESP.restart();  
  }
}

/*
 * Start DNS server
 */
void initDNS() {
  if (dnsServer.start(DNS_PORT, "*", apIP)) {
    Serial.println("DNS server started successfully.");
  } else {
    Serial.println("DNS server start failed.");
  }
}

/*
 * Initialize WebServer
 */
void initWebServer() {
  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", HTTP_GET, handleRoot);                 
  server.on("/configwifi", HTTP_POST, handleConfigWifi);
  server.onNotFound(handleNotFound);                   
 
  server.begin();  
  Serial.println("WebServer started!");
}

/*
 * Scan nearby WiFi networks
 */
bool scanWiFi() {
  Serial.println("Starting WiFi scan...");
  int n = WiFi.scanNetworks();
  Serial.println("Scan complete.");
  if (n == 0) {
    Serial.println("No networks found.");
    scanNetworksID = "No networks found.";
    return false;
  } else {
    Serial.print(n);
    Serial.println(" networks found.");
    for (int i = 0; i < n; ++i) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      scanNetworksID += "<P>" + WiFi.SSID(i) + "</P>";
      delay(10);
    }
    return true;
  }
}

/*
 * Connect to WiFi
 */
void connectToWiFi(int timeOut_s) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid.c_str(), wifi_pass.c_str());

    int connect_time = 0;
    while (WiFi.status() != WL_CONNECTED && connect_time < 2 * timeOut_s) {
        Serial.print(".");
        delay(500);
        connect_time++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nWiFi connection failed, switching back to AP mode!");
        wifiConfig();  
    }
}

/*
 * Switch to AP mode for configuration
 */
void wifiConfig() {
  WiFi.mode(WIFI_AP_STA);
  initSoftAP();   
  initDNS();        
  initWebServer();  
  scanWiFi();       
}

/*
 * Clear stored WiFi credentials
 */
void restoreWiFi() {
  delay(500);
  esp_wifi_restore();
  Serial.println("WiFi credentials erased. Restarting device...");
  delay(10);
}

/*
 * Check WiFi connection status
 */
void checkConnect(bool reConnect) {
  if (WiFi.status() != WL_CONNECTED) {
    if (reConnect && WiFi.getMode() != WIFI_AP && WiFi.getMode() != WIFI_AP_STA) {
      Serial.println("WiFi not connected.");
      Serial.println("Reconnecting...");
      connectToWiFi(30);
    }
  }
}

/*
 * Process DNS and HTTP requests
 */
void checkDNS_HTTP() {
  dnsServer.processNextRequest();   // Handle DNS requests
  server.handleClient();            // Handle HTTP requests
}
