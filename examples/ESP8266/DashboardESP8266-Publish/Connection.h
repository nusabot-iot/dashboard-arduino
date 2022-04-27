long lastReconnectAttempt = 0;
WiFiClient espClient;
Dashboard dashboard(espClient);

void setupDashboard(const char* ssid, const char* password, const char* server, uint16_t port) {
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("WELCOME TO DASHBOARD");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println(WiFi.localIP());
  Serial.println("Connecting to server");
  dashboard.setServer(server, port);
}

void reconnectCheck(String clientId, String authProject) {
  if (!dashboard.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {    // Reconnect every 5 second. (Menghubungkan ulang setiap 5 detik.)
      lastReconnectAttempt = now;
      Serial.print(".");
      if (dashboard.connect(clientId.c_str())) {
        Serial.println("");
        Serial.println("Server Connected");
        authProject += "/+";
        dashboard.subscribe(authProject.c_str());
        lastReconnectAttempt = 0;
      }
    }
  }
}