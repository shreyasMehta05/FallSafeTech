#include <WiFi.h>
#include <PubSubClient.h>
#include <WebServer.h>

const char *ssid = "Inesh5G";
const char *password = "Password";
const char *mqttBroker = "mqtt3.thingspeak.com";
const char *mqttUsername = "OxAwEjcuCA49NwIjGjgoPS4";
const char *mqttClientId = "OxAwEjcuCA49NwIjGjgoPS4";
const char *mqttPassword = "wrR+qodAbvp5pl9QzH3mUQ1Y";

const int ledPin = 2; // Pin connected to LED
const int pirPin = 15; // Pin connected to PIR sensor

int doorState = LOW;
bool toggleEnabled = false; // Variable to control toggle functionality

unsigned long lastPirTriggerTime = 0; // Variable to store the time of last PIR trigger

WiFiClient espClient;
PubSubClient mqttClient(espClient);
WebServer server(80);

// void mqttSubscriptionCallback(char* topic, byte* payload, unsigned int length);

// Declaration of toggleLED function
void toggleLED();
const char htmlContent[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
    <title>LED Control</title>
    <style>
        /* Your CSS styles here */
        body {
            background-color: #00d9ffec;
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            text-align: center;
        }
        h1 {
            margin: 0;
            padding: 1em;
            background-color: #333;
            color: #fff;
        }
        h1 svg{
            margin-left: 0.5em;
            width: 1em;
            color: #fff;
        }
        /* add animation to highlight and glow my heading */
        h1 {
            animation: glow 2s infinite alternate;
        }
        @keyframes glow {
            from {
                text-shadow: 0 0 10px #a6f900, 0 0 20px #00ff44, 0 0 30px #ff0000;
            }
            to {
                text-shadow: 0 0 30px #ffee00, 0 0 50px #00ff00, 0 0 70px #ff0000;
            }
            
        }
        button {
            font-size: 1em;
            padding: 1em;
            margin: 1em;
            background-color: #333;
            color: #fff;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            box-shadow: 0 0 10px #000;
            width: 200px
        }
        button:hover {
            background-color: #555;
        }
        .outerContainerNotify {
            display: flex;
            justify-content: center;
            align-items: center;
            width: 50%;
            margin: 0 auto;
            /* create an outer border and highlight it */
            border: 1px solid #8e8e8e;
            border-radius: 5px;
            padding: .2em;
            background-color: #000;
            /* yellowish tone */
            background-color: #f0e68c;
            box-shadow: 0 0 10px #232323ab;
            margin-top: 1em;
            width:200px;
            box-sizing: border-box;
        }
        #notification {
            font-family: system-ui, -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif;
            font-size: 1em;
            padding: 1em;
            /* margin: 1em; */
            /* use similar stylings */
            background-color: #333;
            color: #000000;
            line-clamp: 1;
            border: 1px solid #f0e68c;
            border-radius: 5px;
            width: 100%;
            background-color: #dbca32;
            box-shadow: 0 0 10px #000;
            /* make font more bolder */
            font-weight:bolder;
            /* make text 3d */
            /* text-shadow: 1px 1px 1px #000 ; */
        }
        .topa{
            position: relative;
            z-index: 1000;
        }
        .topa1{
            position: fixed;
            height: 100px;
        }
        footer {
            /* position: fixed; */

            bottom: 0;
            width: 100%;
            background-color: #333;
            color: #fff;
            padding: 1em;
            display: flex;
            justify-content: center;
            align-items: center;
            /* font-stretch: 1em; */
        }
        .customAlert {
            z-index: 100000;
            display: flex;
            justify-content: center;
            align-items: center;
            width: 50%;
            margin: 0 auto;
            /* create an outer border and highlight it */
            border: 1px solid #8e8e8e;
            border-radius: 5px;
            padding: .2em;
            background-color: #000;
            /* yellowish tone */
            background-color: #f0e68c;
            box-shadow: 0 0 10px #232323ab;
            margin-top: 1em;
            width:200px;
            box-sizing: border-box;
        }
        #fallDetectionAlert {
            font-family: system-ui, -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif;
            font-size: 1em;
            padding: 1em;
            /* margin: 1em; */
            /* use similar stylings */
            background-color: #333;
            color: #000000;
            line-clamp: 1;
            border: 1px solid #f0e68c;
            border-radius: 5px;
            width: 100%;
            background-color: #dbca32;
            box-shadow: 0 0 10px #000;
            /* make font more bolder */
            font-weight:bolder;
            /* make text 3d */
            /* text-shadow: 1px 1px 1px #000 ; */
        }
        * {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
  }
  
  body {
    height: 100vh;
    background: #001219;
    overflow: hidden;
  }
  
  .circle {
    z-index: 1;
    position: absolute;
    bottom: 0;
    width: 20px;
    aspect-ratio: 1/1;
    background: #fca311;
    box-shadow: 0 0 10px #ff5400, 0 0 20px #ff5400, 0 0 30px #ff5400,
      0 0 50px #ff5400;
    border-radius: 50%;
    animation: animate 5s linear forwards;
  }
  
  @keyframes animate {
    0% {
      transform: translatey(0);
      opacity: 0;
    }
    50% {
      opacity: 1;
    }
    100% {
      transform: translatey(-100vh);
      opacity: 1;
    }
  }
  
  .circle::before {
    content: "";
    position: absolute;
    z-index: 1;
    bottom: 100%;
    left: 25%;
    width: 50%;
    height: 100vh;
    opacity: 0.5s;
    background: linear-gradient(#ff5400, transparent);
  }
  
    </style>
    <script>
        function toggleLED() {
            // show a popup to notify the user that the LED is being toggled
            alert('Changing Status for LED');
            fetch('/toggle')
                .then(response => {
                    if (response.ok) {
                        return response.text();
                    }
                    throw new Error('Network response was not ok');
                })
                .then(data => {
                    console.log('Response:', data);
                    // if(data == '1'){
                    //     document.getElementById('notification').innerHTML = 'LED is ON';
                    // } else {
                    //     document.getElementById('notification').innerHTML = 'LED is OFF';
                    // }
                })
                .catch(error => {
                    console.error('Error:', error);
                });
        }
    </script>
    <script>
    // Function to fetch the current status of the door from ThingSpeak
    function getCurrentDoorStatus() {
        fetch('https://api.thingspeak.com/channels/2501193/feeds/last.json?api_key=NJC058SFR2SO32TW')
            .then(response => {
                if (response.ok) {
                    return response.json();
                }   
                throw new Error('Network response was not ok');
            })
            .then(data => {
                console.log('Door Status Response:', data);
                // Update LED control button based on the current status of the door
                if (data.field1 == '1') {
                    // add an emoji of led on
                    document.getElementById('notification').innerHTML = 'LED is ON';
                    document.getElementById('notification').style.backgroundColor = '#00ff00';
                    document.getElementById('notification').style.color = '#333';
                    document.getElementsByClassName('outerContainerNotify')[0].style.backgroundColor = '#000';
                    document.getElementById('notification').style.fontWeight = 'bolder';


                } else {
                    document.getElementById('notification').innerHTML = 'LED is OFF';
                    document.getElementById('notification').style.backgroundColor = '#ff0000';
                    document.getElementById('notification').style.color = '#fff';
                    document.getElementById('notification').style.fontWeight = 'bolder';
                    document.getElementsByClassName('outerContainerNotify')[0].style.backgroundColor = '#000';
                }
            })
            .catch(error => {
                console.error('Error fetching door status:', error);
            });
    }
    function getFallDetectionStatus(){
        fetch('https://api.thingspeak.com/channels/2505280/feeds/last.json?api_key=L4KBHT93ETBIMR9B')
            .then(response => {
                if (response.ok) {
                    return response.json();
                }   
                throw new Error('Network response was not ok');
            })
            .then(data => {
                console.log('Fall Detection Status Response:', data);
                // Update LED control button based on the current status of the door
                if (data.field7 == '2') {
                    // add an emoji of led on
                    document.getElementById('fallDetectionAlert').innerHTML = 'Fall Detected';
                    document.getElementById('fallDetectionAlert').style.backgroundColor = '#ff0000';
                    document.getElementById('fallDetectionAlert').style.color = '#fff';
                    document.getElementsByClassName('customAlert')[0].style.backgroundColor = '#000';
                    document.getElementById('fallDetectionAlert').style.fontWeight = 'bolder';
                } else {
                    document.getElementById('fallDetectionAlert').innerHTML = 'No Fall Detected';
                    document.getElementById('fallDetectionAlert').style.backgroundColor = '#00ff00';
                    document.getElementById('fallDetectionAlert').style.color = '#333';
                    document.getElementById('fallDetectionAlert').style.fontWeight = 'bolder';
                    document.getElementsByClassName('customAlert')[0].style.backgroundColor = '#000';
                }
            })
    }
    setInterval(() => {
        getCurrentDoorStatus();
        // getCurrentPIRStatus();
        getFallDetectionStatus();
    }, 1000);
// </script>

</head>
<body>
    <h1 class="topa">
        Smart Home Dashboard
        <!-- add svg -->
        <svg xmlns="http://www.w3.org/2000/svg" width="1em" height=".8em" fill="currentColor" class="bi bi-house" viewBox="0 0 16 16">
            <path d="M8 0l7 7v7h-4v-6H5v6H1V7l7-7zm1 7h2v6h-2V7z"/>
        </svg>
    </h1>
    <div class="outerContainerNotify topa">
        <div id="notification">
            No Status
        </div>
    </div>
   
    <!-- adding a notifyication element -->
    <button onclick="toggleLED()" class="topa">
        LED CONTROL
        <!-- add svg -->
        <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-lightbulb" viewBox="0 0 16 16">
            <path d="M8 0a1 1 0 0 1 1 1v1h2V1a1 1 0 0 1 1-1h1a1 1 0 0 1 1 1v1h1a2 2 0 0 1 2 2v1h1a1 1 0 0 1 1 1v1h1a2 2 0 0 1 2 2v5a2 2 0 0 1-2 2h-1v1a1 1 0 0 1-1 1h-4a1 1 0 0 1-1-1v-1H6a1 1 0 0 1-1-1V9H4a2 2 0 0 1-2-2V6a2 2 0 0 1 2-2h1V3a1 1 0 0 1 1-1h1V1a1 1 0 0 1 1-1h1zM4 6h8v1H4V6zm8 2H4v5h8V8z"/>
        </svg>  

    </button>
    <!-- add a new div for showing fall detection alert-->
    <div class="customAlert topa">
        <div id="fallDetectionAlert">
            No Fall Detected
        </div>
    </div>

    <footer class="topa topa1" >
        <p>Created by Team 8</p>
    </footer>
</body>
<script>
    function lines() {
        let sizeW = Math.random() * 22;
        let duration = Math.random() * 3;
        let e = document.createElement("div");
        e.setAttribute("class", "circle");
        document.body.appendChild(e);
        e.style.width = 12 + sizeW + "px";
        e.style.left = Math.random() * +innerWidth + "px";
        e.style.animationDuration = 2 + duration + "s";
      
        setTimeout(function () {
          document.body.removeChild(e);
        }, 5000);
      }
      setInterval(function () {
        lines();
      }, 200);
    //   function for inserting graph
    //  showing you don't have access to the thingspeak channel

    // document.getElementsByClassName('graphPlot1')[0].innerHTML = '<h1>Sorry, you do not have access to the thingspeak channel</h1>';
    </script>

</html>
)=====";

void handleRoot() {
    server.send(200, "text/html", htmlContent);
}

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

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Connect to MQTT broker
    mqttClient.setServer(mqttBroker, 1883);
    // mqttClient.setCallback(mqttSubscriptionCallback);

    // Start the web server
    server.on("/", handleRoot);
    server.on("/toggle", HTTP_GET, []() {
        toggleLED();
        server.send(200, "text/plain", String(doorState));
    });
    server.begin();
}
bool lightOn=false;
void loop() {
    mqttClient.loop();
    server.handleClient();

    int pirValue = digitalRead(pirPin);
    unsigned long currentTime = millis();

    if (pirValue == HIGH) {
        // Toggle LED state only if toggleEnabled is false or if last trigger time is more than 5 seconds ago
        // if (!toggleEnabled || (currentTime - lastPirTriggerTime > 2000)) {
        if (!toggleEnabled ) {
            doorState = doorState == HIGH ? LOW : HIGH;
            lastPirTriggerTime = currentTime;
        }
        Serial.println("High");
    } else {
        Serial.println("Low");
    }
    Serial.print("doorState");
    Serial.println(doorState);
    digitalWrite(ledPin, doorState); // Update LED state


    if (mqttClient.connect(mqttClientId, mqttUsername, mqttPassword)) {
        Serial.println("Connected to MQTT broker");
        String topicString = "channels/" + String(2501193) + "/publish";
        // Construct the message payload
        String message = "field1=" + String(doorState);
        // Publish the message to ThingSpeak
        mqttClient.publish(topicString.c_str(), message.c_str());
        Serial.println("Data sent to ThingSpeak");
        // Don't disconnect immediately after publishing
        // mqttClient.disconnect();
    } else {
        Serial.println("Failed to connect to MQTT broker");
    }

    delay(2000); // Wait for 2 seconds before next reading
}

// void mqttSubscriptionCallback(char* topic, byte* payload, unsigned int length) {
//     // Implement MQTT subscription callback if needed
// }

void toggleLED() {
    doorState = doorState == HIGH ? LOW : HIGH; // Toggle LED state
}

