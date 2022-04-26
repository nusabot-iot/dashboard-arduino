#include <ESP8266WiFi.h>
#include <Dashboard.h>

#define BUTTON 12    // Using Button on GPIO12. (Menggunakan Button pada GPIO12)

// Update these with values suitable for your network. (Ubah value berikut sesuai jaringan kamu.)
const char* ssid = "......";
const char* password = "......";
const char* server = "broker.emqx.io";
const char* authProject = "......";

// Set the Client ID with random number. You can change with any Client ID. (Atur Client ID dengan nomor acak. Anda bisa menggantinya dengan Client ID apapun.)
// String CleintId = "YourClientId";
const String clientId = "ClientName-" + String(random(0xffff), HEX);

WiFiClient espClient;

DashboardTimer timer;
char msg[50];
long lastReconnectAttempt = 0;

// Create variable to store message for "if" condition. (Buat variabel untuk menyimpan pesan untuk kondisi "if")
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

  if(String(topic) == String(authProject) + "/led"){
    digitalWrite(BUILTIN_LED, subsMessage.toInt());

    if(subsMessage.toInt() == 1){
      Serial.println("LED ON");
    } else {
      Serial.println("LED OFF");
    }
  }
  subsMessage = "";
}

Dashboard client(server, 1883, subscribe, espClient);

void publish(){
  int pot = digitalRead(BUTTON);
  sprintf(msg, "%ld", pot);
  client.publish("authProject/button", msg);
}

void reconnect() {
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {    // Reconnect every 5 second. (Menghubungkan ulang setiap 5 detik.)
      lastReconnectAttempt = now;
      if (client.connect(clientId.c_str())) {
        client.subscribe("nusabot/+");
        lastReconnectAttempt = 0;
      }
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);       // Initialize the BUILTIN_LED pin as an output (Inisialisasi BUILTIN_LED sebagai output)
  pinMode(BUTTON, INPUT);             // Initialize BUTTON as input for digital sensor. (Inisialisasi BUTTON sebagai input untuk sensor digital.)
  
  Serial.begin(115200);
  setup_wifi();
  timer.setInterval(1000L, publish); //Publish message every 1 second. Non-blocking (Publish pesan setiap 1 detik. Non-blocking)
}

void loop() {
  //Device dissconnected (Perangkat terputus)
  if (!client.connected()) {
    reconnect();
  } else {
    // Device connected (perangkat terhubung)
    client.loop();
    timer.run();

    //==PUT YOUR CODE HERE FOR PROCESS==//
    //==LETAKAN KODE PROGRAM DISINI UNTUK DILAKUKAN PROSES==//
  }
}
