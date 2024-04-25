#include <Client.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <CooperativeMultitasking.h>
#include <ArduinoMqttClient.h>

char ssid[] = "thecloud";
char pass[] = "#######";
char host[] = "146.71.78.42";
char clientid[] = "waterpumparduino";
char username[] = "water";
char password[] = "#####";
char topicname[] = "waterpressure2";
int port = 1883;

CooperativeMultitasking tasks;
Continuation beginWiFiIfNeeded;
Continuation connectMQTTClientIfNeeded;
Continuation light;
Continuation dark;
Guard isDark;
Continuation on;
Continuation off;
Guard isLight;

WiFiClient wificlient;
//MQTTClient mqttclient(&tasks, &wificlient, host, 1883, clientid, username, password);
//MQTTTopic topic(&mqttclient, topicname);

MqttClient mqttClient(wificlient);

void setup() {
  Serial.begin(9600);
  //
  //while (!Serial) {
  //  delay(1000);
  //}
  //
  Serial.println("begin wifi");
  //
  WiFi.begin(ssid, pass);
  tasks.after(3000, beginWiFiIfNeeded); // after 10 seconds call beginWiFiIfNeeded()
  //
  Serial.println("connect mqtt client");
  //
  mqttClient.setUsernamePassword(username,password);
  
if (!mqttClient.connect(host, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }
  mqttClient.beginMessage(topicname);
  mqttClient.print("connected");
  mqttClient.endMessage();
  pinMode(LED_BUILTIN, OUTPUT);
  //tasks.now(light); // call light() now
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
      Serial.print("retrying wifi");
      delay(4000);
    }   
  }
  
  if (!mqttClient.connected()) {
    Serial.println("mqtt client not connected");
    
    mqttClient.connect(host, port);
  }
    
  //tasks.run();
  int sensor;
  int sensorsum=0;
  int loops =20;
  for (int i =0; i<loops;i++){
    sensorsum = sensorsum + analogRead(A1);
    delay(10);
  }
 sensor=sensorsum/loops;
 
  float voltage = sensor * (5.0 / 1023.0);
  int psi;
  psi = map(sensor,157,873,0,100);
  //Serial.println(sensor);
//  Serial.println(" input");

 // Serial.print(voltage);
//  Serial.println(" volts");

  //Serial.print(psi);
  //Serial.println(" psi");
  String header = "raw,voltage,calculated\n";
  //String body = String(sensor).c_str()+","+String(voltage).c_str() + "," + String(psi).c_str();
  String body = String(sensor)+","+String(voltage) + "," + psi;
  //String message = header+body;
  String message = body;
  mqttClient.beginMessage(topicname);
  //mqttClient.print(String(psi).c_str());
  mqttClient.print(message);
  Serial.println(message);
  mqttClient.endMessage();
  delay(1000);
}


void beginWiFiIfNeeded() {
  switch (WiFi.status()) {
    case WL_IDLE_STATUS:
    case WL_CONNECTED: tasks.after(30000, beginWiFiIfNeeded); return; // after 30 seconds call beginWiFiIfNeeded() again
    case WL_NO_SHIELD: Serial.println("no wifi shield"); return; // do not check again
    case WL_CONNECT_FAILED: Serial.println("wifi connect failed"); break;
    case WL_CONNECTION_LOST: Serial.println("wifi connection lost"); break;
    case WL_DISCONNECTED: Serial.println("wifi disconnected"); break;
  }
  //
  WiFi.begin(ssid, pass);
  tasks.after(5000, beginWiFiIfNeeded); // after 10 seconds call beginWiFiIfNeeded() again
}

void connectMQTTClientIfNeeded() {
  if (!mqttClient.connected()) {
    Serial.println("mqtt client not connected");
    
    mqttClient.connect(host, port);
  }
  
  tasks.after(30000, connectMQTTClientIfNeeded); // after 30 seconds call connectMQTTClientIfNeeded() again
}
