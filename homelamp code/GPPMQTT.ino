#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include "arduinosecrets.ino"

// WiFi & MQTT Settings
const char* ssid = "";
const char* password = "";
const char* mqtt_server = "";

WiFiClient espClient;
PubSubClient client(espClient);

// LED Strip Settings
#define LED_PIN 5
#define NUM_LEDS 20
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

#define THRESHOLD 600  // Activation threshold

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");

    client.setServer(mqtt_server, 1884);
    client.setCallback(callback);

    while (!client.connected()) {
        Serial.println("Connecting to MQTT...");
        if (client.connect("ESP32_B_Actuator")) {
            Serial.println("Connected to MQTT!");
            client.subscribe("esp32/sensor");  // Subscribe to sensor data
        } else {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            delay(5000);
        }
    }

    strip.begin();
    strip.clear();
    strip.show();
}

// MQTT Message Handling
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message received: ");
    
    char jsonBuffer[100];  
    memcpy(jsonBuffer, payload, length);
    jsonBuffer[length] = '\0';  

    Serial.println(jsonBuffer);  // Debug received message

    StaticJsonDocument<100> doc;
    DeserializationError error = deserializeJson(doc, jsonBuffer);

    if (error) {
        Serial.println("JSON Parsing Failed!");
        return;
    }

    int fsrValue = doc["FSR"];
    Serial.print("Parsed FSR Value: ");
    Serial.println(fsrValue);

    if (fsrValue > THRESHOLD) {
        gradualTurnOff();
    } else {
        gradualLightUp();
    }
}

void loop() {
    client.loop();  // Keep MQTT connection alive
}

// Gradually turn LEDs on
void gradualLightUp() {
    for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, strip.Color(237, 228, 15));
        strip.show();
        delay(50);
    }
}

// Gradually turn LEDs off
void gradualTurnOff() {
    for (int i = NUM_LEDS - 1; i >= 0; i--) {
        strip.setPixelColor(i, strip.Color(0, 0, 0)); 
        strip.show();
        delay(50);
    }
}
