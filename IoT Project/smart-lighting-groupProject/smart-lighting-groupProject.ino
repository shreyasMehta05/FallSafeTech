#include <WiFi.h>
#include <PubSubClient.h>

const char *ssid = "Inesh5G";
const char *password = "Password";
const char *mqttBroker = "mqtt3.thingspeak.com";
const char *mqttUsername = "OxAwEjcuCA49NwIjGjgoPS4";
const char *mqttClientId = "OxAwEjcuCA49NwIjGjgoPS4";
const char *mqttPassword = "wrR+qodAbvp5pl9QzH3mUQ1Y";

const int ledPin = 2; // Pin connected to LED
const int pirPin = 15; // Pin connected to PIR sensor

int doorState = LOW;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
  delay(2500);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Connect to MQTT broker
  client.setServer(mqttBroker, 1883);
}
void loop() {
  int pirValue = digitalRead(pirPin);

  if (pirValue == HIGH) {
    Serial.println("High");
    digitalWrite(ledPin, HIGH); // Turn on LED
    doorState = HIGH;
  } else {
    Serial.println("Low");
    digitalWrite(ledPin, LOW); // Turn off LED
    doorState = LOW;
  }

  if (client.connect(mqttClientId, mqttUsername, mqttPassword)) {
    Serial.println("Connected to MQTT broker");
    String topicString = "channels/" + String(2501193) + "/publish";
    // Construct the message payload
    String message = "field1=" + String(doorState);
    // Publish the message to ThingSpeak
    client.publish(topicString.c_str(), message.c_str());
    Serial.println("Data sent to ThingSpeak");
    // Don't disconnect immediately after publishing
    // client.disconnect();
  } else {
    Serial.println("Failed to connect to MQTT broker");
  }

  delay(1500); // Wait for 1.5 seconds before next reading
}
