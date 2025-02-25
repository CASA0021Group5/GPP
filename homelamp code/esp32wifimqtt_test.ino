#include <WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "";
const char* password = "";

// MQTT broker settings
const char* mqtt_server = ""; // Your MQTT broker address
const int mqtt_port = ;  // MQTT port (adjust if needed)
const char* mqtt_user = "";
const char* mqtt_pass = "";

// MQTT topics
const char* topic_receive = "student/1H1L/rec"; // Subscribe topic
const char* topic_send = "student/1H1L/snd";   // Publish topic

int value = 0; 

WiFiClient espClient;
PubSubClient client(espClient);

// WiFi connection function
void setup_wifi() {
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");
}

// Reconnect to MQTT broker if disconnected
void reconnect() {
    while (!client.connected()) {
        Serial.print("Connecting to MQTT...");
        if (client.connect("ESP32Client", mqtt_user, mqtt_pass)) { // Use authentication
            Serial.println("Connected!");
            client.subscribe(topic_receive); // Subscribe to topic
        } else {
            Serial.print("Failed, State: ");
            Serial.print(client.state());
            Serial.println(" Retrying in 5 seconds...");
            delay(5000);
        }
    }
}

// Callback function when a message is received
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message received [");
    Serial.print(topic);
    Serial.print("]: ");

    // Convert payload to a string
    String message = "";
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.println(message);
}

void send() {
    value = 1;
    client.publish(topic_send, String(value).c_str());
}

void setup() {
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop(); // Keep MQTT connection alive
    send();
    delay(5000);
}
