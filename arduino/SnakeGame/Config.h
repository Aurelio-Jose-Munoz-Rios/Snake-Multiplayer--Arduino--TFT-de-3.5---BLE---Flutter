#ifndef CONFIG_H
#define CONFIG_H

// WiFi
#define WIFI_SSID "FamiliaMuri"
#define WIFI_PASSWORD "F4m1l14Mur!"
// WebSocket
#define WS_HOST "192.168.1.173"
#define WS_PORT 8765

// BLE
#define BLE_NAME "ESP32-SNAKE"
#define SERVICE_UUID "fc96f65e-318a-4001-84bd-77e9d12af44b"
#define CHARACTERISTIC_UUID_RX "04d3552e-b9b3-4be6-a8b4-aa43c4507c4d"

// DFPlayer Mini (Serial2)
#define DFPLAYER_RX 16
#define DFPLAYER_TX 17

// DAC para efectos
#define DAC_PIN 25  // GPIO25 = DAC1

// Juego
#define GAME_WIDTH 26
#define GAME_HEIGHT 14
#define MAX_SNAKE_LENGTH 200
#define MAX_OBSTACLES 100
#define BASE_SPEED 200
#define MIN_SPEED 60
#define POINTS_PER_LEVEL 50

// Audio
#define SAMPLE_RATE 8000
#define WAVE_SAMPLES 32

#endif