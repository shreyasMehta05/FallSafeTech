#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <Server.h>
#include <PubSubClient.h>
#include <ThingSpeak.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

#define mqttPort 1883

// char ssid[] = "OnePlus Nord 3 5G";
// char pass[] = "qwerty415";

char ssid[] = "Inesh5G";
char pass[] = "Password";

// Ensure that the credentials here allow you to publish and subscribe to the ThingSpeak channel.
const long channelID = 2505280;  // Replace 123456 with your actual ThingSpeak channel number 
const char mqttUserName[] = "KjszDAcHIi4GDAoUBw4AEBE";
const char clientID[] = "KjszDAcHIi4GDAoUBw4AEBE";
const char mqttPass[] = "QL3NU9wOYJ3Xr/DVF0bzEIve";


WiFiClient client;
const char* server = "mqtt3.thingspeak.com";
int status = WL_IDLE_STATUS;
long lastPublishMillis = 0;
int connectionDelay = 1;
int updateInterval = 3;

PubSubClient mqttClient(client);

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println("yo");
  connectWifi();
  mqttClient.setServer(server, mqttPort);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}
int state = 0 ;
float a_prev = -1e7;
float a_net = 0;
float w_net = 0;
float threshold1 = 5;
float thresholdtheta = 0 ;
float thresholdangular = 0;
float threshold_acc = 0;
float threshold_w = 0;

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  if (WiFi.status() != WL_CONNECTED) {
    connectWifi();
  }
  if (!mqttClient.connected()) {
    mqttConnect();
    mqttSubscribe(channelID);
  }
  mqttClient.loop();
  a_prev = a.acceleration.x - 9.95182;
  delay(500);
  Serial.print("Acceleration X: ");
  a.acceleration.x = a.acceleration.x - 9.95182;
  a.acceleration.y = a.acceleration.y + 0.2715;
  a.acceleration.z = a.acceleration.z - 0.314;
  g.gyro.x = g.gyro.x - 0.0071 ;
  g.gyro.y =g.gyro.y + 0.0077 ;
  g.gyro.z = g.gyro.z + 0.0153 ;
  
  a_net = sqrt(a.acceleration.x*a.acceleration.x + a.acceleration.y*a.acceleration.y + a.acceleration.z*a.acceleration.z);
  w_net = sqrt(g.gyro.x*g.gyro.x + g.gyro.y*g.gyro.y + g.gyro.z*g.gyro.z);
  if(state==0 && abs(a_net - a_prev) > threshold1)
  {
    delay(500);
    a.acceleration.x = a.acceleration.x - 9.95182;
    a.acceleration.y = a.acceleration.y + 0.2715;
    a.acceleration.z = a.acceleration.z - 0.314;
    g.gyro.x = g.gyro.x - 0.0071 ;
    g.gyro.y =g.gyro.y + 0.0077 ;
    g.gyro.z = g.gyro.z + 0.0153 ;
    float w = sqrt(g.gyro.x*g.gyro.x + g.gyro.y*g.gyro.y + g.gyro.z*g.gyro.z);
    float theta = (w+w_net)/4 ;
    float angular_acc = 2*(w-w_net);
    if(abs(theta) > thresholdtheta || abs(angular_acc) > thresholdangular)
    {
      state = 1;
    }

  }
  else if(state==1)
  {
    if(a_net > threshold_acc && w_net > threshold_w)
    {
      state = 2;
    }
    else
    {
      state = 0;
    }
  }
  else
  {
    Serial.println("Fall Detected");
    delay(3000);
    state = 0;
  }
  if (abs(long(millis()) - lastPublishMillis) > updateInterval * 1000) {
    String topicString = "channels/" + String(channelID) + "/publish";

    String message = "&field1=" + String(a.acceleration.x);
    message += "&field2=" + String(a.acceleration.y);
    message += "&field3=" + String(a.acceleration.z);
    message += "&field4=" + String(g.gyro.x);
    message += "&field5=" + String(g.gyro.y);
    message += "&field6=" + String(g.gyro.z);
    message += "&field7=" + String(state);
    mqttClient.publish(topicString.c_str(), message.c_str());
      lastPublishMillis = millis();
  }
}
void mqttConnect() {
  while (!mqttClient.connected()) {
    if (mqttClient.connect(clientID, mqttUserName, mqttPass)) {
      Serial.println("MQTT connected.");
    } else {
      Serial.print("MQTT connection failed, rc=");
      Serial.println(mqttClient.state());
      delay(5000);
    }
  }
}
void connectWifi() {
  Serial.println("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, pass);
    delay(1000);
    Serial.println(".");
  }
  Serial.println("Connected to Wi-Fi");
}
void mqttSubscribe(long subChannelID) {
  String myTopic = "channels/" + String(subChannelID) + "/subscribe";
  mqttClient.subscribe(myTopic.c_str());
}
