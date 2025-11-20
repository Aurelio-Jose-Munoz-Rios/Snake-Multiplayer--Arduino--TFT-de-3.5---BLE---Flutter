#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include <DFRobotDFPlayerMini.h>
#include <HardwareSerial.h>
#include <driver/dac.h>
#include "Config.h"

class MusicPlayer {
private:
  DFRobotDFPlayerMini player;
  HardwareSerial* serial;
  dac_channel_t dacChannel;
  bool initialized;
  
  // Onda triangular para efecto de comer (suave y agradable)
  const uint8_t triangleWave[WAVE_SAMPLES] = {
    0, 16, 32, 48, 64, 80, 96, 112,
    128, 144, 160, 176, 192, 208, 224, 240,
    255, 240, 224, 208, 192, 176, 160, 144,
    128, 112, 96, 80, 64, 48, 32, 16
  };
  
  void playDACWave(const uint8_t* wave, int freq, int duration) {
    int samples = (SAMPLE_RATE * duration) / 1000;
    int period = SAMPLE_RATE / freq;
    
    for (int i = 0; i < samples; i++) {
      int index = (i * WAVE_SAMPLES / period) % WAVE_SAMPLES;
      dac_output_voltage(dacChannel, wave[index]);
      delayMicroseconds(1000000 / SAMPLE_RATE);
    }
    dac_output_voltage(dacChannel, 128);
  }

public:
  MusicPlayer() : initialized(false) {
    serial = new HardwareSerial(2);
    dacChannel = DAC_CHANNEL_1; // GPIO 25
  }
  
  void begin() {
    // Inicializar DFPlayer
    serial->begin(9600, SERIAL_8N1, DFPLAYER_RX, DFPLAYER_TX);
    delay(500);
    
    if (player.begin(*serial)) {
      initialized = true;
      player.volume(25);
      delay(100);
      Serial.println("DFPlayer Mini OK");
    } else {
      Serial.println("Error DFPlayer Mini");
      initialized = false;
    }
    
    // Inicializar DAC
    dac_output_enable(dacChannel);
    dac_output_voltage(dacChannel, 128);
    Serial.println("DAC inicializado en GPIO 25");
  }
  
  void startBackgroundMusic() {
    if (!initialized) return;
    
    // Reproducir 0001 en modo loop
    player.loop(1);
    delay(50);
    Serial.println("Música de fondo iniciada (0001 loop)");
  }
  
  void playEatEffect() {
    // Efecto de "burbuja" ascendente usando DAC
    for (int f = 400; f <= 1000; f += 100) {
      playDACWave(triangleWave, f, 20);
    }
    Serial.println("Efecto comer reproducido (DAC)");
  }
  
  void playDeathEffect() {
    // Efecto descendente con DAC
    for (int f = 800; f >= 200; f -= 100) {
      playDACWave(triangleWave, f, 30);
    }
    Serial.println("Efecto muerte reproducido (DAC)");
  }
  
  void playLevelUpEffect() {
    // Arpeggio ascendente
    int notes[] = {440, 554, 659, 880};
    for (int i = 0; i < 4; i++) {
      playDACWave(triangleWave, notes[i], 60);
      delay(20);
    }
  }
  
  void stopMusic() {
    if (!initialized) return;
    player.pause();
  }
  
  void resumeMusic() {
    if (!initialized) return;
    player.start();
  }
  
  void setVolume(int vol) {
    if (!initialized) return;
    if (vol < 0) vol = 0;
    if (vol > 30) vol = 30;
    player.volume(vol);
  }
  
  bool isInitialized() {
    return initialized;
  }
};

#endif
