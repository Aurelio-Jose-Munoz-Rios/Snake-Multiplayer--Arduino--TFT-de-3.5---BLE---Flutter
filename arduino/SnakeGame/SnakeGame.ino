#include "Config.h"
#include "Point.h"
#include "BLEController.h"
#include "WebSocketManager.h"
#include "MusicPlayer.h"
#include "SnakeGame.h"

BLEController bleController;
WebSocketManager wsManager;
MusicPlayer musicPlayer;
SnakeGame game;

bool musicStarted = false;

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  
  Serial.println("\n=== Snake 2 Jugadores ===");
  Serial.println("DFPlayer: Música de fondo");
  Serial.println("DAC GPIO25: Efectos de sonido");
  
  musicPlayer.begin();
  delay(1000);
  
  bleController.begin();
  wsManager.begin();
  game.init();
  
  Serial.println("Sistema listo");
}

void loop() {
  wsManager.loop();
  
  // Iniciar música cuando se conecte al WebSocket
  if (wsManager.isConnected() && !musicStarted && musicPlayer.isInitialized()) {
    musicPlayer.startBackgroundMusic();
    musicStarted = true;
  }
  
  Direction cmd1 = bleController.getPlayer1Command();
  Direction cmd2 = bleController.getPlayer2Command();
  
  if (cmd1 != DIR_NONE) {
    if (!game.hasStarted()) {
      game.start();
    }
    game.setPlayer1Direction(cmd1);
  }
  
  if (cmd2 != DIR_NONE) {
    if (!game.hasStarted()) {
      game.start();
    }
    game.setPlayer2Direction(cmd2);
  }
  
  if (game.hasStarted() && !game.hasEnded()) {
    bool levelUp = false;
    bool p1Ate = false;
    bool p2Ate = false;
    
    game.update(levelUp, p1Ate, p2Ate);
    
    // Efecto de comer con DAC
    if (p1Ate || p2Ate) {
      musicPlayer.playEatEffect();
    }
    
    // Subir nivel
    if (levelUp) {
      musicPlayer.playLevelUpEffect();
      Serial.printf("¡NIVEL %d!\n", game.getLevel());
    }
    
    wsManager.sendJSON(game.toJSON());
    
    // Game Over
    if (game.hasEnded()) {
      musicPlayer.playDeathEffect();
      
      String msg = "{\"type\":\"gameover\",\"totalScore\":" + String(game.getTotalScore()) + 
                   ",\"level\":" + String(game.getLevel()) + "}";
      wsManager.sendJSON(msg);
      
      Serial.println("Game Over");
      
      delay(3000);
      game.init();
      
      // La música 0001 sigue en loop automáticamente
    }
    
    delay(game.getSpeed());
  } else {
    delay(100);
  }
}
