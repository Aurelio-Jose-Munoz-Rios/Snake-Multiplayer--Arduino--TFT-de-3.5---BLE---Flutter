#ifndef BLE_CONTROLLER_H
#define BLE_CONTROLLER_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include "Config.h"
#include "Point.h"

class BLEController : public BLEServerCallbacks {
private:
  BLEServer* server;
  BLECharacteristic* rxCharacteristic;
  volatile Direction player1Command;
  volatile Direction player2Command;
  int connectedClients;
  static BLEController* instance;
  
  class CommandCallback : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* characteristic) {
      uint8_t* data = characteristic->getData();
      if (characteristic->getLength() > 0) {
        Direction cmd = (Direction)data[0];
        
        // Determinar a qué jugador pertenece el comando
        if (cmd >= 1 && cmd <= 4) {
          BLEController::instance->player1Command = cmd;
          Serial.printf("P1 comando: %d\n", cmd);
        } else if (cmd >= 11 && cmd <= 14) {
          BLEController::instance->player2Command = cmd;
          Serial.printf("P2 comando: %d\n", cmd);
        }
      }
    }
  };

public:
  BLEController() : server(nullptr), rxCharacteristic(nullptr), 
                    player1Command(DIR_NONE), player2Command(DIR_NONE),
                    connectedClients(0) {
    instance = this;
  }
  
  // Callbacks de conexión/desconexión
  void onConnect(BLEServer* pServer) override {
    connectedClients++;
    Serial.printf("Cliente conectado. Total: %d\n", connectedClients);
    
    // No detener el advertising para permitir más conexiones
    if (connectedClients < 2) {
      BLEDevice::startAdvertising();
      Serial.println("Esperando segundo jugador...");
    }
  }
  
  void onDisconnect(BLEServer* pServer) override {
    connectedClients--;
    Serial.printf("Cliente desconectado. Total: %d\n", connectedClients);
    
    // Reiniciar advertising si hay menos de 2 clientes
    BLEDevice::startAdvertising();
  }
  
  void begin() {
    BLEDevice::init(BLE_NAME);
    
    server = BLEDevice::createServer();
    server->setCallbacks(this);
    
    // Crear servicio
    BLEService* service = server->createService(SERVICE_UUID);
    
    // Crear característica
    rxCharacteristic = service->createCharacteristic(
      CHARACTERISTIC_UUID_RX,
      BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR
    );
    
    rxCharacteristic->addDescriptor(new BLE2902());
    rxCharacteristic->setCallbacks(new CommandCallback());
    
    service->start();
    
    // Configurar advertising
    BLEAdvertising* advertising = BLEDevice::getAdvertising();
    advertising->addServiceUUID(SERVICE_UUID);
    advertising->setScanResponse(true);
    advertising->setMinPreferred(0x06);
    advertising->setMaxPreferred(0x12);
    
    BLEDevice::startAdvertising();
    
    Serial.println("BLE iniciado - Esperando 2 jugadores");
  }
  
  Direction getPlayer1Command() {
    Direction cmd = player1Command;
    player1Command = DIR_NONE;
    return cmd;
  }
  
  Direction getPlayer2Command() {
    Direction cmd = player2Command;
    player2Command = DIR_NONE;
    return cmd;
  }
  
  int getConnectedClients() const {
    return connectedClients;
  }
  
  bool bothPlayersConnected() const {
    return connectedClients >= 2;
  }
};

BLEController* BLEController::instance = nullptr;

#endif
