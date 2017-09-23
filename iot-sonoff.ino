//
//   SONOFF MQTT v1.0 - Ricardo Merces - twitter.com/r_merces 
//

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// ----- GPIO 12 SONOFF -----
#define RELAY 12  // GPIO PIN SONOFF BASIC

// ----- WIFI & MQTT SETTINGS -----
const char* ssid = "wifi network";
const char* password = "wifi password";
const char* mqtt_server = "192.168.1.4";
const int   mqtt_port = 1883;
const char* mqtt_topic = "home/light/office";
const char* mqtt_clientID = "sonoffs01";
const char* mqtt_user = "mqtt username";
const char* mqtt_pass = "mqtt password";
// --------------------------------

WiFiClient espClient;
PubSubClient client(espClient);

// ----- SETUP -----
void setup() {
  pinMode(RELAY, OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

// ----- SETUP WIFI -----
void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

// ----- CALLBACK -----
void callback(char* topic, byte* payload, unsigned int length) {
  if ((char)payload[0] == '1') {
    digitalWrite(RELAY, HIGH);   // Turn the RELAY ON 
  }
  if ((char)payload[0] == '0') {
    digitalWrite(RELAY, LOW);   // Turn the RELAY OFF
  }
}

// ----- RECONNECT MQTT -----
void reconnect_mqtt() {
  while (!client.connected()) {
    client.connect(mqtt_clientID, mqtt_user, mqtt_pass); // REMOVE THE MQTT_USER AND MQTT_PASS IF NECESSARY 
    client.subscribe(mqtt_topic);
    delay(1000);
  }
}

// ----- LOOP -----
void loop() {
  if (!client.connected()) {
    reconnect_mqtt();
  }
  client.loop();
}
