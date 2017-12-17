#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
const char *ssid =  "";   // nama ssid dari wi-fi, jangan lebih dari 32 karakter
const char *pass =  "";   // password dari wi-fi
 
//------------------------------ Konfigurasi server pada <a href="https://www.cloudmqtt.com/">https://www.cloudmqtt.com/</a> 
// perhatikan akun CloudMQTT yang telah dibuat, kemudian pastikan konfigurasi yang diperlukan ini, sama dengan akun yang anda miliki.
const char *mqtt_server = "m14.cloudmqtt.com";
const int mqtt_port = 18622;
const char *mqtt_user = "jztmmefb";
const char *mqtt_pass = "3Hm_ftWsUQwE";
//------------------------------ End Konfigurasi server pada <a href="https://www.cloudmqtt.com/">https://www.cloudmqtt.com/</a>
 
//deklarasi variabel untuk led 
int led = 16;
int led2 = 4;
 
//deklarasi konfigurasi server
WiFiClient wclient;
PubSubClient client(wclient, mqtt_server, mqtt_port);
 
//------------------------------ fungsi untuk menerima nilai balik (subcribe)
void callback(const MQTT::Publish& pub) {
  if(pub.payload_string() == "1")
  {
    digitalWrite(led, HIGH);
    client.publish("/led/state", "Lampu Hidup");
  }
  else if(pub.payload_string() == "0")
  {
    digitalWrite(led, LOW);
    client.publish("/led/state", "Lampu Mati");
  }
  
  Serial.println(pub.payload_string()); 
}
//------------------------------ End fungsi untuk menerima nilai balik (subcribe)
 
void setup() {
  // Setup console
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);
}
 
void loop() {
  //----------------------------- cek apakah wi-fi sudah tersambung
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);
 
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    Serial.println("WiFi connected");
  }
  //----------------------------- End cek apakah wi-fi sudah tersambung
 
  //----------------------------- cek apakah ESP sudah tersambung dengan server
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      Serial.println("Connecting to MQTT server");
      if (client.connect(MQTT::Connect("arduinoClient2").set_auth(mqtt_user, mqtt_pass))) {
        Serial.println("Connected to MQTT server");
        client.set_callback(callback);
        client.subscribe("/led");
        client.publish("/led/state", "0");
      } else {
        Serial.println("Could not connect to MQTT server");   
      }
    }
    //----------------------------- cek apakah ESP sudah tersambung dengan server
 
    if (client.connected())
      client.loop();
  }
}
