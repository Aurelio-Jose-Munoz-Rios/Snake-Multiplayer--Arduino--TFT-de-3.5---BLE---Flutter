#ifndef WEBSOCKET_MANAGER_H
#define WEBSOCKET_MANAGER_H

#include <WiFi.h>
#include <WebSocketsClient.h>
#include "Config.h"

class WebSocketManager {
private:
  WebSocketsClient client;
  bool isConnectedToServer;
  
  static WebSocketManager* instance;
  
  static void eventCallback(WStype_t type, uint8_t* payload, size_t length) {
    if (type == WStype_CONNECTED) {
      Serial.println("[WS] Conectado al servidor");
      if (instance) {
        instance->isConnectedToServer = true;
      }
    } else if (type == WStype_DISCONNECTED) {
      Serial.println("[WS] Desconectado");
      if (instance) {
        instance->isConnectedToServer = false;
      }
    }
  }

public:
  WebSocketManager() : isConnectedToServer(false) {
    instance = this;
  }
  
  void begin() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Conectando WiFi");
    
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    
    Serial.println("\nWiFi OK");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    
    client.begin(WS_HOST, WS_PORT, "/");
    client.onEvent(eventCallback);
    client.setReconnectInterval(5000);
  }
  
  void loop() {
    client.loop();
  }
  
  void sendJSON(String json) {
    client.sendTXT(json);
  }
  
  bool isConnected() {
    return isConnectedToServer;
  }
};

WebSocketManager* WebSocketManager::instance = nullptr;

#endif
