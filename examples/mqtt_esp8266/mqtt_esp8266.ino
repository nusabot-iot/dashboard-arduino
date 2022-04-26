#include <ESP8266WiFi.h>
#include <Dashboard.h>

#define POTENTOIMETER A0    // Using potentiometer as example. (Menggunakan potentiometer sebagai contoh)

// Update these with values suitable for your network.
// Ubah value berikut sesuai jaringan kamu.
const char* ssid = "........";
const char* password = "........";
const char* server = "broker.emqx.io";
String authProject = "........";
String subscribeTopic = "........";

// Set the Client ID with random number. You can change with any Client ID.
// Atur Client ID dengan nomor acak. Anda bisa menggantinya dengan Client ID apapun.
// String CleintId = "YourClientId";
String clientId = "ClientName-" + String(random(0xffff), HEX);

WiFiClient espClient;
Dashboard client(espClient);
char msg[50];
long lastReconnectAttempt = 0;

// Create variable to store message for "if" condition
// Buat variabel untuk menyimpan pesan untuk kondisi "if"
String subsMessage;

void setup_wifi() {
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
}

void subscribe(char* topic, byte* payload, unsigned int length) {
  for (int i = 0; i < length; i++) {
    subsMessage = subsMessage + (char)payload[i];
  }

  if(String(topic) == authProject + "/" + subscribeTopic + "/led"){
    digitalWrite(BUILTIN_LED, subMessage.toInt());
  }
  subMessage = "";
}

void publish(){
  int pot = analogRead(POTENTOIMETER);

  unsigned long now = millis();
  if (now - lastMsg > 2000) {   //Send data every 2 seconds (kirim data setiap 2 detik)
    lastMsg = now;
    sprintf(msg, "%ld", pot);
    client.publish(authProject + "/" + subscribeTopic + "/pot", msg);
  }
}

boolean reconnect() {
  if (client.connect(clientId)) {
    client.subscribe(authProject + "/" + subscribeTopic + "/+");
  }
  return client.connected();
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);       // Initialize the BUILTIN_LED pin as an output (Inisialisasi BUILTIN_LED sebagai output)
  pinMode(POTENTIOMETER, INPUT)       // Initialize A0 as input for analog sensor. You can use potentiometer fo example. (Inisialisasi A0 sebagai input untuk sensor analog. Anda dapat menggunakan )
  Serial.begin(115200);
  setup_wifi();
  client.setServer(server, 1883);
  client.setCallback(subscribe);
}

void loop() {
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    // Device connected (perangkat terhubung)
    client.loop();
    publish();
  }
}
