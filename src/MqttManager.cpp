#include "MQTTManager.h"

MQTTManager::MQTTManager(const char* server, WiFiClient& wifiClient) : server(server), client(wifiClient) {

  client.setServer(server, 1883);
}

void MQTTManager::connect(const char* topic1, const char* topic2, const char* topic3) {
  client.setKeepAlive(60);
  while (!client.connected()) {
    String clientId = "Client-151354";

    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado ao servidor MQTT.");
          
        if (topic1) client.subscribe(topic1);
        if (topic2) client.subscribe(topic2);
        if (topic3) client.subscribe(topic3);

    } else {
      Serial.print("Falha na conexão, estado: ");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void MQTTManager::setCallback(MQTT_CALLBACK_SIGNATURE) {
  client.setCallback(callback);
}

void MQTTManager::loop() {
  client.loop();
}

void MQTTManager::publishMessage(const char* topic, const char* message) {
  client.publish(topic, message);
}


// bool MQTTManager::testMsg(byte* payload, const char* msg, size_t length){
    
//   if (strncmp((char*)payload, msg , length) == 0){
//     return true;
//   }
// }
