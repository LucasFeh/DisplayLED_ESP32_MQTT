#include "WiFiManager.h"

WiFiManager::WiFiManager(const char* ssid, const char* password) : ssid(ssid), password(password) {}

void WiFiManager::connect() {
  Serial.println("Conectando ao Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Conectado ao Wi-Fi.");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}
