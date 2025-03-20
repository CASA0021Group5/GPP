#include "WiFiUser.h"
#include "mqtt.h"
// Creating an Adafruit_NeoPixel object
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
// MQTT broker settings
const char* mqtt_server = "0.tcp.eu.ngrok.io";  // Your MQTT broker address
const int mqtt_port = 15912;                    // MQTT port (adjust if needed)
const char* mqtt_user = SECRET_MQTTUSER;
const char* mqtt_pass = SECRET_MQTTPASS;
const char* client_id = MQTTCLIENT_ID;
// MQTT topics
const char* topic_send = TOPIC_S;     // Publish topic
const char* topic_receive = TOPIC_R;  // Subscribe topic

WiFiClient espClient;
PubSubClient client(espClient);

// leds setting
int t1;           //Time of other led turned on
int t2;           //Time of self led turned on
int mqttvalue;    //The number which will be sent to mqtt server
int mqttsave;     //status changing check variable
int mqttrevalue;  //retained message from mqtt

bool ledAnimating;
bool ledTurningOff;
int ledIndex;
int offIndex;

unsigned long previousMillis = 0;
unsigned long offMillis = 0;
const int interval = 120;
const int offInterval = 50;

//AP and wifi setting
const byte DNS_PORT = 53;  // Set DNS port
const int webPort = 80;    // Set Web port

const char* AP_SSID = "ESP32-4_1";  // Set AP SSID
//const char* AP_PASS  = "";               // No password for AP

const char* HOST_NAME = "MY_ESP32";  // Set device name
String scanNetworksID = "";          // Store scanned WiFi SSIDs

IPAddress apIP(192, 168, 4, 1);  // Set AP IP address

String wifi_ssid = "";  // Temporarily store WiFi SSID
String wifi_pass = "";  // Temporarily store WiFi password

DNSServer dnsServer;        // Create DNS server instance
WebServer server(webPort);  // Start web server

#define ROOT_HTML "<!DOCTYPE html><html><head><title>WIFI</title><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><style type=\"text/css\">.input{display: block; margin-top: 10px;}.input span{width: 100px; float: left; float: left; height: 36px; line-height: 36px;}.input input{height: 30px;width: 200px;}.btn{width: 120px; height: 35px; background-color: #000000; border:0px; color:#ffffff; margin-top:15px; margin-left:100px;}</style><body><form method=\"POST\" action=\"configwifi\"><label class=\"input\"><span>WiFi SSID</span><input type=\"text\" name=\"ssid\" value=\"\"></label><label class=\"input\"><span>WiFi PASS</span> <input type=\"text\"  name=\"pass\"></label><input class=\"btn\" type=\"submit\" name=\"submit\" value=\"Submit\"> <p><span> Nearby wifi:</P></form>"

/*
 * Handling requests for access to the root directory of the website
 */
void handleRoot() {
  if (server.hasArg("selectSSID")) {
    server.send(200, "text/html", ROOT_HTML + scanNetworksID + "</body></html>");  //scanNetWprksID is the scaned wifi
  } else {
    server.send(200, "text/html", ROOT_HTML + scanNetworksID + "</body></html>");
  }
}

/*
 * Reminder page after submitting data
 */
void handleConfigWifi()  //Return http status
{
  if (server.hasArg("ssid"))  //Determine if there is an account parameter
  {
    Serial.print("got ssid:");
    wifi_ssid = server.arg("ssid");  //Get the content of the html form input box name named ‘ssid’

    Serial.println(wifi_ssid);
  } else  //no parameter
  {
    Serial.println("error, not found ssid");
    server.send(200, "text/html", "<meta charset='UTF-8'>error, not found ssid");  //Back to error page
    return;
  }
  if (server.hasArg("pass")) {
    Serial.print("got password:");
    wifi_pass = server.arg("pass");  //Get the content of the html form input box name ‘pwd’
    Serial.println(wifi_pass);
  } else {
    Serial.println("error, not found password");
    server.send(200, "text/html", "<meta charset='UTF-8'>error, not found password");
    return;
  }
  server.send(200, "text/html", "<meta charset='UTF-8'>SSID：" + wifi_ssid + "<br />password:" + wifi_pass + "<br />WiFi information has been obtained, trying to connect, please close this page manually.");  //返回保存成功页面
  delay(2000);
  WiFi.softAPdisconnect(true);  //parameter is set to true, the device will directly switch off the access point mode, i.e. switch off the WiFi network established by the device.
  server.close();               //Shutting down web services
  WiFi.softAPdisconnect();      //Calling this function without parameters will disable the access point mode and set the currently configured AP hotspot network name and password to null values.
  Serial.println("WiFi Connect SSID:" + wifi_ssid + "  PASS:" + wifi_pass);

  if (WiFi.status() != WL_CONNECTED)  //WIFI connected failed
  {
    Serial.println("Start calling the connection function connectToWiFi()...");
    connectToWiFi(connectTimeOut_s);
  } else {
    Serial.println("The submitted configuration information automatically connects successfully...");
  }
}

/*
 * Function ‘handleNotFound’ for handling 404 cases
 */
void handleNotFound()  // This custom function handles when the web resource requested by the browser cannot be found on the server
{
  handleRoot();  //Accessing a non-existent directory returns you to the configuration page
  //   server.send(404, "text/plain", "404: Not found");
}

/*
 * AP mode
 */
void initSoftAP() {
  WiFi.mode(WIFI_AP);                                          //Configure for AP mode
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));  //Setting the AP hotspot IP and subnet mask
  if (WiFi.softAP(AP_SSID))                                    //Enable the AP hotspot, add the second parameter if you need a password
  {
    //Print information
    Serial.println("ESP-32S SoftAP is right.");
    Serial.print("Soft-AP IP address = ");
    Serial.println(WiFi.softAPIP());                                             //Access point ip
    Serial.println(String("MAC address = ") + WiFi.softAPmacAddress().c_str());  //Access point mac
  } else                                                                         //Failure to turn on AP hotspot
  {
    Serial.println("WiFiAP Failed");
    delay(1000);
    Serial.println("restart now...");
    ESP.restart();  //Reboot reset esp32
  }
}

/*
 * Enabling DNS servers
 */
void initDNS() {
  if (dnsServer.start(DNS_PORT, "*", apIP))  //Determine if mapping all addresses to the esp32's ip was successful or not
  {
    Serial.println("start dnsserver success.");
  } else {
    Serial.println("start dnsserver failed.");
  }
}

/*
 * Initialising the WebServer
 */
void initWebServer() {
  if (MDNS.begin("esp32"))  //Set the domain name esp32 for the device, the full domain name is esp32.local
  {
    Serial.println("MDNS responder started");
  }
  //You must add a second parameter, HTTP_GET, in the following format, otherwise you can't force the portal to work.
  server.on("/", HTTP_GET, handleRoot);                   //  When the browser requests the server root directory (website home page) call the custom function handleRoot processing, set the home page callback function, you must add the second parameter HTTP_GET, otherwise you can not force the portal
  server.on("/configwifi", HTTP_POST, handleConfigWifi);  //  The custom function handleConfigWifi is called to handle when the browser requests the server/configwifi (form field) directory

  server.onNotFound(handleNotFound);  //Call the custom function handleNotFound when the web resource requested by the browser cannot be found on the server

  server.begin();  //Starting the TCP SERVER

  Serial.println("WebServer started!");
}

/*
 * Scanning for nearby WiFi in order to display it in the distribution interface
 */
bool scanWiFi() {
  Serial.println("scan start");
  Serial.println("--------->");
  // Scanning for nearby WiFi
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    Serial.println("no networks found");
    scanNetworksID = "no networks found";
    return false;
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
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
 * Connecting to WiFi
 */
void connectToWiFi(int timeOut_s) {
  WiFi.hostname(HOST_NAME);  //Setting the device name
  Serial.println("Enter connectToWiFi() function");
  WiFi.mode(WIFI_STA);          //Set to STA mode and connect to WIFI
  WiFi.setAutoReconnect(true);  //Setting up automatic connection

  if (wifi_ssid != "")  //wifi_ssid is not null, meaning that the wifi is read from the webpage
  {
    Serial.println("Connect with web configuration information.");
    WiFi.begin(wifi_ssid.c_str(), wifi_pass.c_str());  //c_str(), get the pointer to this string
    wifi_ssid = "";
    wifi_pass = "";
  } else  //Not read from webpage to wifi
  {
    Serial.println("Connect with information saved by nvs.");
    WiFi.begin();  //begin() does not pass parameters, defaults to the last successfully connected wifi
  }

  int Connect_time = 0;                  //Used to time the connection and reset the device if the connection is unsuccessful for a long time
  while (WiFi.status() != WL_CONNECTED)  //Wait for the WIFI connection to be successful
  {
    Serial.print(".");  //Prints a total of 30 dots

    delay(500);
    Connect_time++;

    if (Connect_time > 2 * timeOut_s)  //Can't connect for a long time, re-enter the matching page
    {

      Serial.println("");  //The main purpose is for line breaks
      Serial.println("WIFI autoconnect fail, start AP for webconfig now...");
      wifiConfig();  //Start the distribution function
      return;        //Jump out to prevent infinite initialisation
    }
  }

  if (WiFi.status() == WL_CONNECTED)  //Connected successfully
  {
    Serial.println("WIFI connect Success");
    Serial.printf("SSID:%s", WiFi.SSID().c_str());
    Serial.printf(", PSW:%s\r\n", WiFi.psk().c_str());
    Serial.print("LocalIP:");
    Serial.print(WiFi.localIP());
    Serial.print(" ,GateIP:");
    Serial.println(WiFi.gatewayIP());
    Serial.print("WIFI status is:");
    Serial.println(WiFi.status());
    server.stop();  //Stop the web server created by the development board
    WIFILED();      //turn on all leds when WIFI connnected
  }
}

/*
 * Configure the distribution network functions
 */
void wifiConfig() {
  initSoftAP();
  initDNS();
  initWebServer();
  scanWiFi();
}


/*
 * Delete the saved wifi information, here the deletion is to delete the information stored in flash. After deleting, wifi can't read the record of the last connection, and need to rewire the network
 */
void restoreWiFi() {
  delay(500);
  esp_wifi_restore();  //Delete saved wifi information
  Serial.println("Connection information cleared, ready to reboot device...");
  delay(10);
}

/*
 * Check WiFi connection status, reconnect if disconnected and integret mqtt connection functions, mearsuring sensor value functions and MQTT message pulish functions.
 */
void checkConnect(bool reConnect) {
  if (WiFi.status() != WL_CONNECTED)  // WIFI connnected failed
  {
    Serial.println("WIFI disconnected!");
    delay(5000);

    if (reConnect == true && WiFi.getMode() != WIFI_AP && WiFi.getMode() != WIFI_AP_STA) {
      Serial.println("WIFI not connected.");
      Serial.println("WiFi Mode:");
      Serial.println(WiFi.getMode());
      Serial.println("Connecting to WiFi...");

      connectToWiFi(connectTimeOut_s);  // Function to let board connecting to WIFI
      return;
    }
  } else {

    if (!client.connected()) {
      Serial.println("MQTT disconnected, trying to reconnect...");
      reconnectmqtt();
    }
    client.loop();
    Self_FSR_loop();
    Home_MQTT_LED();
    updateLEDAnimation();
    updateLEDTurnOff();
  }
}

/*
 * Process DNS and HTTP requests
 */

void checkDNS_HTTP() {
  dnsServer.processNextRequest();  // Handle DNS requests
  server.handleClient();           // Handle HTTP requests
}

/*
 * FSR and led strip
 */
void LEDinit() {
  pinMode(FSR_PIN, INPUT);
  strip.begin();
  strip.fill(strip.Color(0, 0, 0));
  strip.show();
}

void WIFILED() {
  strip.fill(strip.Color(80, 80, 80));
  strip.show();
  delay(3000);
  strip.fill(strip.Color(0, 0, 0));
  strip.show();
}

void Ledtrunon() {
  strip.setPixelColor(7, 120, 110, 0);
  strip.show();
}

void Ledtrunoff() {
  strip.setPixelColor(7, 0, 0, 0);
  strip.show();
}

void graduallyLightUp() {
  //Serial.println("Received '1', turn on the LEDS");
  if (!ledAnimating) {  // when animation not is activated, initialize ledindex
    ledIndex = 10;
  }
  ledAnimating = true;
  ledTurningOff = false;
}

void updateLEDAnimation() {
  if (!ledAnimating) return;  // if ledanimating is false then end this function

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (ledIndex == 10) {
      if (t2 < 50){
      strip.setPixelColor(ledIndex + 12, 10, 10, 10);
      strip.setPixelColor(ledIndex + 13, 100, 100, 100);
      strip.setPixelColor(ledIndex, 100, 100, 100);
      strip.setPixelColor(ledIndex + 1, 100, 100, 100);
      strip.setPixelColor(ledIndex + 2, 10, 10, 10);

      strip.setPixelColor(ledIndex + 11, 0, 0, 0);
    }else{
      strip.setPixelColor(ledIndex + 12, 1, 1, 1);
      strip.setPixelColor(ledIndex + 13, 10, 10, 10);
      strip.setPixelColor(ledIndex, 10, 10, 10);
      strip.setPixelColor(ledIndex + 1, 10, 10, 10);
      strip.setPixelColor(ledIndex + 2, 1, 1, 1);

      strip.setPixelColor(ledIndex + 11, 0, 0, 0);
    }
    }
    if (ledIndex == 11) {
      if (t2 < 50){
      strip.setPixelColor(ledIndex + 12, 10, 10, 10);
      strip.setPixelColor(ledIndex - 1, 100, 100, 100);
      strip.setPixelColor(ledIndex, 100, 100, 100);
      strip.setPixelColor(ledIndex + 1, 100, 100, 100);
      strip.setPixelColor(ledIndex + 2, 10, 10, 10);

      strip.setPixelColor(ledIndex + 11, 0, 0, 0);
      }else{
      strip.setPixelColor(ledIndex + 12, 1, 1, 1);
      strip.setPixelColor(ledIndex - 1, 10, 10, 10);
      strip.setPixelColor(ledIndex, 10, 10, 10);
      strip.setPixelColor(ledIndex + 1, 10, 10, 10);
      strip.setPixelColor(ledIndex + 2, 1, 1, 1);

      strip.setPixelColor(ledIndex + 11, 0, 0, 0);
      }
    }

    if (ledIndex == 12) {
      if (t2 < 50){
      strip.setPixelColor(ledIndex - 2, 10, 10, 10);
      strip.setPixelColor(ledIndex - 1, 100, 100, 100);
      strip.setPixelColor(ledIndex, 100, 100, 100);
      strip.setPixelColor(ledIndex + 1, 100, 100, 100);
      strip.setPixelColor(ledIndex + 2, 10, 10, 10);

      strip.setPixelColor(ledIndex + 11, 0, 0, 0);
    }else{
      strip.setPixelColor(ledIndex - 2, 1, 1, 1);
      strip.setPixelColor(ledIndex - 1, 10, 10, 10);
      strip.setPixelColor(ledIndex, 10, 10, 10);
      strip.setPixelColor(ledIndex + 1, 10, 10, 10);
      strip.setPixelColor(ledIndex + 2, 1, 1, 1);

      strip.setPixelColor(ledIndex + 11, 0, 0, 0);
    }
    }

    // Turn on current one
    if (ledIndex > 12 && ledIndex <= 21) {
      if (t2 < 50){
      strip.setPixelColor(ledIndex - 2, 10, 10, 10);
      strip.setPixelColor(ledIndex - 1, 100, 100, 100);
      strip.setPixelColor(ledIndex, 100, 100, 100);
      strip.setPixelColor(ledIndex + 1, 100, 100, 100);
      strip.setPixelColor(ledIndex + 2, 10, 10, 10);

      strip.setPixelColor(ledIndex - 3, 0, 0, 0);
      }else{  // night mode when t2 >= 50
      strip.setPixelColor(ledIndex - 2, 1, 1, 1);
      strip.setPixelColor(ledIndex - 1, 10, 10, 10);
      strip.setPixelColor(ledIndex, 10, 10, 10);
      strip.setPixelColor(ledIndex + 1, 10, 10, 10);
      strip.setPixelColor(ledIndex + 2, 1, 1, 1);

      strip.setPixelColor(ledIndex - 3, 0, 0, 0);
      }
    }


    if (ledIndex == 22) {
      if (t2 < 50){
      strip.setPixelColor(ledIndex - 2, 10, 10, 10);
      strip.setPixelColor(ledIndex - 1, 100, 100, 100);
      strip.setPixelColor(ledIndex, 100, 100, 100);
      strip.setPixelColor(ledIndex + 1, 100, 100, 100);
      strip.setPixelColor(ledIndex - 12, 10, 10, 10);

      strip.setPixelColor(ledIndex - 3, 0, 0, 0);
    }else{
      strip.setPixelColor(ledIndex - 2, 1, 1, 1);
      strip.setPixelColor(ledIndex - 1, 10, 10, 10);
      strip.setPixelColor(ledIndex, 10, 10, 10);
      strip.setPixelColor(ledIndex + 1, 10, 10, 10);
      strip.setPixelColor(ledIndex - 12, 1, 1, 1);

      strip.setPixelColor(ledIndex - 3, 0, 0, 0);
    }
    }

    if (ledIndex == 23) {
      if (t2 < 50){
      strip.setPixelColor(ledIndex - 2, 10, 10, 10);
      strip.setPixelColor(ledIndex - 1, 100, 100, 100);
      strip.setPixelColor(ledIndex, 100, 100, 100);
      strip.setPixelColor(ledIndex - 13, 100, 100, 100);
      strip.setPixelColor(ledIndex - 12, 10, 10, 10);

      strip.setPixelColor(ledIndex - 3, 0, 0, 0);
    }else{
      strip.setPixelColor(ledIndex - 2, 1, 1, 1);
      strip.setPixelColor(ledIndex - 1, 10, 10, 10);
      strip.setPixelColor(ledIndex, 10, 10, 10);
      strip.setPixelColor(ledIndex - 13, 10, 10, 10);
      strip.setPixelColor(ledIndex - 12, 1, 1, 1);

      strip.setPixelColor(ledIndex - 3, 0, 0, 0);
    }
    }
    strip.show();
    // move to next one
    ledIndex++;
    t2++;
    if (ledIndex > NUM_LEDS) {
      ledAnimating = false;  // end led animating
    }
  }
}

void graduallyTurnOff() {
  //Serial.println("Turning off LEDs gradually");
  if (!ledTurningOff) {  // when animation not is activated, initialize offindex
    offIndex = 10;
  }
  ledAnimating = false;
  ledTurningOff = true;
  t2 = 0;
}

void updateLEDTurnOff() {
  if (!ledTurningOff) return;

  unsigned long currentMillis = millis();
  if (currentMillis - offMillis >= offInterval) {
    offMillis = currentMillis;

    strip.setPixelColor(offIndex, 0, 0, 0);
    strip.show();

    offIndex++;
    if (offIndex >= NUM_LEDS) {
      ledTurningOff = false;
    }
  }
}

int getStableFSRValue() {
  int sum = 0;
  for (int i = 0; i < 6; i++) {
    sum += analogRead(FSR_PIN);
    delay(10);
  }
  return sum / 6;
}

void Self_FSR_loop() {
  int sensorValue = getStableFSRValue();
  if (sensorValue > THRESHOLD) {
    Ledtrunoff();
    mqttvalue = 0;
  } else {
    Ledtrunon();
    mqttvalue = 1;
  }

  if (mqttvalue != mqttsave) {
    String payload = String(mqttvalue);
    if (client.publish(topic_send, payload.c_str(), true)) {
      Serial.println("publish successful");
    } else {
      Serial.println("publish failed");
    }
  };
  mqttsave = mqttvalue;
}

void Home_MQTT_LED() {
  if (mqttrevalue == 1) {
    graduallyLightUp();
  } else if (mqttrevalue == 0) {
    graduallyTurnOff();
  } else {
    Serial.println("Unknown message received!");
  }
}

/*
 * Mqtt Server Functions
 */
void reconnectmqtt() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect(client_id, mqtt_user, mqtt_pass)) {  // Use authentication
      Serial.println("Connected!");
      client.subscribe(topic_receive);  // Subscribe to topic
    } else {
      Serial.print("Failed, State: ");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message received [");
  Serial.print(topic);
  Serial.print("]: ");

  // Convert payload to a string
  String Mqtt_message = "";
  for (int i = 0; i < length; i++) {
    Mqtt_message += (char)payload[i];
  }
  Serial.println(Mqtt_message);
  mqttrevalue = Mqtt_message.toInt();
}

void mqttsetup() {
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  if (client.connect(client_id, mqtt_user, mqtt_pass)) {
    Serial.println("MQTT Connected! :D");
    client.subscribe(topic_receive);
  }
  mqttsave = 9;
  t1 = 0;
  t2 = 0;
  ledAnimating = false;
  ledTurningOff = false;
  ledIndex = 10;
  offIndex = 10;
}

/*
 * backup code
 */
// void gradualLightUp() {
//   Serial.println("Received '1', turn on the LEDS");
//   if (t2 < 500) {
//     for (int i = 23; i < NUM_LEDS - 1; i = i + 2) {
//       if (i > 23) {
//         strip.setPixelColor(i - 2, 0, 0, 0);
//       }
//       strip.setPixelColor(i, 150, 150, 150);
//       strip.show();
//       delay(80);
//     }
//   } else {
//     for (int i = 23; i < NUM_LEDS - 1; i = i + 2) {
//       if (i > 23) {
//         strip.setPixelColor(i - 2, 0, 0, 0);
//       }
//       strip.setPixelColor(i, 10, 10, 10);
//       strip.show();
//       delay(80);
//     }
//   }
//   strip.setPixelColor(NUM_LEDS - 2, 0, 0, 0);
//   strip.show();
//   t2++;
// }

// void gradualTurnOff() {
//   t2 = 0;
//   for (int i = 23; i < NUM_LEDS; i++) {
//     strip.setPixelColor(i, 0, 0, 0);
//     strip.show();
//     delay(50);
//   }
// }