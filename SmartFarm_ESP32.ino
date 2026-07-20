#include <WiFi.h>
#include <PubSubClient.h>
#include "HX711.h"

// --- USER NETWORK SETTINGS (User must fill this) ---
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// --- MQTT SERVER SETTINGS (Hardcoded for Smart Farm App) ---
const char* mqtt_server = "132.29.230.65";
const int mqtt_port = 1883;
const char* mqtt_user = "iot_node"; 
const char* mqtt_pass = "farm1234"; 

// --- HARDWARE PINS ---
const int LOADCELL_DOUT_PIN = 16;
const int LOADCELL_SCK_PIN = 4;
const int PUMP_PIN = 5;

WiFiClient espClient;
PubSubClient client(espClient);
HX711 scale;

String deviceID = "";

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void get_mac_address() {
  // Get MAC Address to generate a unique Device ID
  String mac = WiFi.macAddress();
  mac.replace(":", ""); // Remove colons
  deviceID = "ESP32-" + mac;
  
  Serial.print("============================\n");
  Serial.print("YOUR DEVICE ID IS: ");
  Serial.println(deviceID);
  Serial.print("Please enter this ID into the App!\n");
  Serial.print("============================\n");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(deviceID.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      // Subscribe to pump command topic (From App)
      String pumpTopic = "farm/" + deviceID + "/pump";
      client.subscribe(pumpTopic.c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  // Receive pump command from mobile app
  if (message == "ON") {
    digitalWrite(PUMP_PIN, HIGH);
    Serial.println("PUMP TURNED ON!");
  } else if (message == "OFF") {
    digitalWrite(PUMP_PIN, LOW);
    Serial.println("PUMP TURNED OFF!");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);

  setup_wifi();
  get_mac_address(); // Generate automatic Device ID

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  // scale.set_scale(CALIBRATION_FACTOR); // Need to calibrate based on your load cell
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Send weight data every 5 seconds
  static unsigned long lastMsg = 0;
  unsigned long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    
    float weight = scale.get_units(10); // Read weight value
    if(weight < 0) weight = 0.00;

    // Build JSON Payload to send to app
    String payload = "{\"berat\":" + String(weight, 2) + "}";
    String sensorTopic = "farm/" + deviceID + "/sensors";
    
    client.publish(sensorTopic.c_str(), payload.c_str());
    Serial.print("Data sent: ");
    Serial.println(payload);
  }
}
