
// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library

#include <ESP8266WiFi.h>
#include <Dashboard.h>
#include <DHT.h>
#include "Connection.h"

#define DHTPIN 4      // GPIO 4 for DHT pin. (GPIO 4 untuk pin DHT.)

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Update these with values suitable for your network. (Ubah value berikut sesuai jaringan kamu.)
const char* ssid = "......";
const char* password = "......";
const char* server = "broker.emqx.io";
const String authProject = "......";

// Set the Client ID with random number. You can change with any Client ID. (Atur Client ID dengan nomor acak. Anda bisa menggantinya dengan Client ID apapun.)
// String CleintId = "YourClientId";
const String clientId = "ClientName-" + String(random(0xffff), HEX);

DHT dht(DHTPIN, DHTTYPE);
DashboardTimer timer;

// Create variable to store size of payload. (Buat variabel untuk menyimpan ukuran payload.)
char msg[50];

void publish(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  sprintf(msg, "%lf", h);
  dashboard.publish(authProject + "/humidity", msg);
  sprintf(msg, "%lf", t);
  dashboard.publish(authProject + "/temperature", msg);
}

void setup() {
  dht.begin();
  
  Serial.begin(115200);
  setupDashboard(ssid, password, server, 1883);
  timer.setInterval(1000L, publish);            // Publish message every 1 second. Non-blocking (Publish pesan setiap 1 detik. Non-blocking)
}

void loop() {
  reconnectCheck(clientId, authProject);        //Check device are connected to the server. (Periksa apakah perangkat terhubung ke server.)
  dashboard.loop();
  timer.run();

  //==PUT YOUR CODE HERE FOR PROCESS==//
  //==LETAKAN KODE PROGRAM DISINI UNTUK DILAKUKAN PROSES==//
}