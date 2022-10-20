
#include <WiFi.h>
#include <PubSubClient.h>

//wifi value
const char* ssid = "404 Network Unavailable";
const char* password = "omfgomfg";

//mqtt value
const char* mqtt_server = "broker.mqttdashboard.com";
//const char *mqtt_username = "mqttwatercontrol";
//const char *mqtt_password = "Abc@12345678";
//const int mqtt_port = 8883;

WiFiClient espClient;
PubSubClient client(espClient);

long now = millis();
char text;

// Motor L
int motorRPin1 = 13; // (IN1)
int motorRPin2 = 27; // (IN2)
int enableRPin = 12; // (ENA)
// Motor R
int motorLPin1 = 25;  // (IN3)
int motorLPin2 = 33;  // (IN4)
int enableLPin = 26;  //  (ENB)
const int freq = 50000;
const int pwmChannel = 0;
const int pwmChanne2 = 1;
const int resolution = 8;

long duration ;
long cm = 100;
int output = 0;
int direc = 0;


#define PIN_RED    23 // GIOP23
#define PIN_GREEN  22 // GIOP22
#define PIN_BLUE   21 // GIOP21

const int freqcolor = 255;
const int ledChannelR = 2;
const int ledChannelG = 3;
const int ledChannelB = 4;
const int resolutioncolor = 8;

////ultrasonic////
const int pingPin = 5;
int inPin = 18;



void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}


void callback(String topic, byte* message, unsigned int length) {

  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  if (topic == "device/led") {
    if (messageTemp == "red") {
      Serial.print("text :");
      Serial.println("led red");
      ledcWrite(ledChannelR,   150);
    }
    else if (messageTemp == "closer") {
      Serial.print("text :");
      Serial.println("close red");
      ledcWrite(ledChannelR,   0);
    }
    else if (messageTemp == "green") {
      Serial.print("text :");
      Serial.println("led green");
      ledcWrite(ledChannelG, 150);
    }
    else if (messageTemp == "closeg") {
      Serial.print("text :");
      Serial.println("close green");
      ledcWrite(ledChannelG, 0);
    }
    else if (messageTemp == "blue") {
      Serial.print("text :");
      Serial.println("led blue");
      ledcWrite(ledChannelB,  150);
    }
    else if (messageTemp == "closeb") {
      Serial.print("text :");
      Serial.println("close blue");
      ledcWrite(ledChannelB,  0);
    }
  }
  else if (topic == "device/car") {
    if (messageTemp == "run") {
      Serial.print("text :");
      Serial.println("run");
      output = 1;
      direc = 1;
      /*   digitalWrite(motorRPin1, HIGH);
         digitalWrite(motorRPin2, LOW);
         ledcWrite(pwmChannel, 255);
         digitalWrite(motorLPin1, LOW);
         digitalWrite(motorLPin2, HIGH);
         ledcWrite(pwmChanne2, 255);*/
    }

    else if (messageTemp == "back") {
      Serial.print("text :");
      Serial.println("back");
      output = 2;
      direc = 1;
      /* digitalWrite(motorRPin1, LOW);
        digitalWrite(motorRPin2, HIGH);
        ledcWrite(pwmChannel, 255);
        digitalWrite(motorLPin1, HIGH);
        digitalWrite(motorLPin2, LOW);
        ledcWrite(pwmChanne2, 255);*/
    }
    else if (messageTemp == "left") {
      Serial.print("text :");
      Serial.println("left");
      output = 3;
      direc = 1;
      /* digitalWrite(motorRPin1, LOW);
        digitalWrite(motorRPin2, HIGH);
        ledcWrite(pwmChannel, 255);
        digitalWrite(motorLPin1, LOW);
        digitalWrite(motorLPin2, HIGH);
        ledcWrite(pwmChanne2, 255);*/
    }
    else if (messageTemp == "right") {
      Serial.print("text :");
      Serial.println("right");
      output = 4;
      direc = 1;
      /* digitalWrite(motorRPin1, HIGH);
        digitalWrite(motorRPin2, LOW);
        ledcWrite(pwmChannel, 255);
        digitalWrite(motorLPin1, HIGH);
        digitalWrite(motorLPin2, LOW);
        ledcWrite(pwmChanne2, 255);*/
    }
    else if (messageTemp == "stop") {
      Serial.print("text :");
      Serial.println("stop");
      output = 0;
      direc = 1;
      /* digitalWrite(motorRPin1, LOW);
        digitalWrite(motorRPin2, LOW);
        ledcWrite(pwmChannel, 0);
        digitalWrite(motorLPin1, LOW);
        digitalWrite(motorLPin2, LOW);
        ledcWrite(pwmChanne2, 0);*/
    }
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    String client_id = "clientId-ZvVAsLBrVd";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
    if (client.connect(client_id.c_str())) {
      // if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Public emqx mqtt broker connected");
      client.subscribe("device/led");
      client.subscribe("device/car");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



void setup() {



  pinMode(motorRPin1, OUTPUT);
  pinMode(motorRPin2, OUTPUT);
  pinMode(enableRPin, OUTPUT);
  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(enableRPin, pwmChannel);

  pinMode(motorLPin1, OUTPUT);
  pinMode(motorLPin2, OUTPUT);
  pinMode(enableLPin, OUTPUT);
  ledcSetup(pwmChanne2, freq, resolution);
  ledcAttachPin(enableLPin, pwmChanne2);


  ledcSetup(ledChannelR, freqcolor, resolutioncolor);
  ledcSetup(ledChannelG, freqcolor, resolutioncolor);
  ledcSetup(ledChannelB, freqcolor, resolutioncolor);

  ledcAttachPin(PIN_RED, ledChannelR);
  ledcAttachPin(PIN_GREEN, ledChannelG);
  ledcAttachPin(PIN_BLUE, ledChannelB);

  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);


  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

// For this project, you don't need to change anything in the loop function. Basically it ensures that you ESP is connected to your broker
void loop() {
  long cm = 100;
  if (!client.connected()) {
    reconnect();
  }
  if (!client.loop())
    client.connect("ESP8266Client");

  now = millis();
  // Publishes new temperature and humidity every 30 seconds
  if (Serial.available() > 0) { //ตรวจสอบว่ามีข้อมูลเข้ามาหรือไม่
    text = Serial.read();
    Serial.print("DEC:");
    Serial.println(text);
    delay(100);
    if (text == 65) {
      client.publish("device/sensor", "oloLee");
    }
    if (text == 66) {
      client.publish("device/car", "back");
    }

  }

  pinMode(pingPin, OUTPUT);
  pinMode(inPin, INPUT);


  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  duration = pulseIn(inPin, HIGH);

  cm = microsecondsToCentimeters(duration);
  static char cm2[7];
  dtostrf(cm, 6, 2, cm2);
  /*  Serial.print("cm :  ");
    Serial.println(cm2);*/
  client.publish("device/sensor", cm2);


  if (output == 0) {
    Serial.println("หยุด");
    digitalWrite(motorRPin1, LOW);
    digitalWrite(motorRPin2, LOW);
    ledcWrite(pwmChannel, 0);
    digitalWrite(motorLPin1, LOW);
    digitalWrite(motorLPin2, LOW);
    ledcWrite(pwmChanne2, 0);
  }
  else if (output == 1 ) {
    digitalWrite(motorRPin1, HIGH);
    digitalWrite(motorRPin2, LOW);
    ledcWrite(pwmChannel, 255);
    digitalWrite(motorLPin1, LOW);
    digitalWrite(motorLPin2, HIGH);
    ledcWrite(pwmChanne2, 255);
  }
  else if (output == 2 ) {
    Serial.println("ถอย");
    digitalWrite(motorRPin1, LOW);
    digitalWrite(motorRPin2, HIGH);
    ledcWrite(pwmChannel, 200);
    digitalWrite(motorLPin1, HIGH);
    digitalWrite(motorLPin2, LOW);
    ledcWrite(pwmChanne2, 200);
  }
  else  if (output == 3 ) {
    Serial.println("เลี้ยวซ้าย");
    digitalWrite(motorRPin1, LOW);
    digitalWrite(motorRPin2, HIGH);
    ledcWrite(pwmChannel, 150);
    digitalWrite(motorLPin1, LOW);
    digitalWrite(motorLPin2, HIGH);
    ledcWrite(pwmChanne2, 150);
  }
  else if (output == 4 ) {
    Serial.println("เลี้ยวขวา");
    digitalWrite(motorRPin1, HIGH);
    digitalWrite(motorRPin2, LOW);
    ledcWrite(pwmChannel, 150);
    digitalWrite(motorLPin1, HIGH);
    digitalWrite(motorLPin2, LOW);
    ledcWrite(pwmChanne2, 150);
    //}

  }
}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}
