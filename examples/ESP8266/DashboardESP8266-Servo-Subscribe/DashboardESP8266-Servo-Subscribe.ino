#include <ESP8266WiFi.h>
#include <Servo.h>
#include <Dashboard.h>
#include "Connection.h"

// Update these with values suitable for your network. (Ubah value berikut sesuai jaringan kamu.)
const char* ssid = "......";
const char* password = "......";
const char* server = "broker.emqx.io";
const String authProject = "......";

// Set the Client ID with random number. You can change with any Client ID. (Atur Client ID dengan nomor acak. Anda bisa menggantinya dengan Client ID apapun.)
// String CleintId = "YourClientId";
const String clientId = "ClientName-" + String(random(0xffff), HEX);

// Create variable to store message for "if" condition. (Buat variabel untuk menyimpan pesan untuk kondisi "if")
String subsMessage;

Servo myservo;

void subscribe(char* topic, byte* payload, unsigned int length) {
  for (int i = 0; i < length; i++) {
    subsMessage = subsMessage + (char)payload[i];
  }

  if(String(topic) == authProject + "/servo"){
    myservo.write(subsMessage.toInt());
  }
  subsMessage = "";
}

void setup() {
  myservo.attach(4);                            // attaches the servo on GIO4 to the servo object

  Serial.begin(115200);
  setupDashboard(ssid, password, server, 1883);
  dashboard.setCallback(subscribe);             // Set device as subscriber. (Atur perangkat sebagai subscriber.)
}

void loop() {
  reconnectCheck(clientId, authProject);        //Check device are connected to the server. (Periksa apakah perangkat terhubung ke server.)
  dashboard.loop();

  //==PUT YOUR CODE HERE FOR PROCESS==//
  //==LETAKAN KODE PROGRAM DISINI UNTUK DILAKUKAN PROSES==//
}