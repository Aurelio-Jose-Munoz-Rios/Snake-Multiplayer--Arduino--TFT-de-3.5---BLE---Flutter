#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include "Config.h"
#include "Point.h"

class Snake {
public:
  Point body[MAX_SNAKE_LENGTH];
  int length;
  Point direction;
  int score;
  bool alive;
  
  Snake() : length(3), score(0), alive(true) {
    direction = Point(1, 0);
  }
  
  void init(int startX, int startY, int dirX, int dirY) {
    length = 3;
    score = 0;
    alive = true;
    body[0] = Point(startX, startY);
    body[1] = Point(startX - dirX, startY - dirY);
    body[2] = Point(startX - 2*dirX, startY - 2*dirY);
    direction = Point(dirX, dirY);
  }
  
  void setDirection(int dx, int dy) {
    if (direction.x != -dx || direction.y != -dy) {
      direction = Point(dx, dy);
    }
  }
  
  void move() {
    for (int i = length - 1; i > 0; i--) {
      body[i] = body[i-1];
    }
    body[0].x += direction.x;
    body[0].y += direction.y;
  }
  
  void grow() {
    length++;
    score += 10;
  }
  
  bool checkSelfCollision() {
    for (int i = 1; i < length; i++) {
      if (body[0].equals(body[i])) {
        return true;
      }
    }
    return false;
  }
};

class SnakeGame {
private:
  Snake player1;
  Snake player2;
  Point obstacles[MAX_OBSTACLES];
  int obstacleCount;
  Point food;
  int level;
  bool gameOver;
  bool isStarted;
  
  // CORRECCIÓN 1 y 2: Comida no aparece en obstáculos ni en zona prohibida
  void placeFood() {
    bool validPosition;
    do {
      validPosition = true;
      
      // Genera posición aleatoria (excluyendo primera fila para HUD)
      food = Point(random(0, GAME_WIDTH), random(2, GAME_HEIGHT)); // Y >= 2
      
      // Check player 1
      for (int i = 0; i < player1.length; i++) {
        if (player1.body[i].equals(food)) {
          validPosition = false;
          break;
        }
      }
      
      // Check player 2
      if (validPosition) {
        for (int i = 0; i < player2.length; i++) {
          if (player2.body[i].equals(food)) {
            validPosition = false;
            break;
          }
        }
      }
      
      // Check obstáculos
      if (validPosition) {
        for (int i = 0; i < obstacleCount; i++) {
          if (obstacles[i].equals(food)) {
            validPosition = false;
            break;
          }
        }
      }
      
    } while (!validPosition);
  }
  
  void generateObstacles(int lvl) {
    obstacleCount = 0;
    int centerX = GAME_WIDTH / 2;
    int centerY = GAME_HEIGHT / 2;
    
    // Ajustar obstáculos para evitar la zona del HUD (Y < 2)
    switch(lvl) {
      case 1:
        break;
        
      case 2:
        for (int i = -2; i <= 2; i++) {
          if (centerY + i >= 2) {
            obstacles[obstacleCount++] = Point(centerX + i, centerY);
            obstacles[obstacleCount++] = Point(centerX, centerY + i);
          }
        }
        break;
        
      case 3:
        for (int i = 0; i < 3; i++) {
          if (i >= 2) obstacles[obstacleCount++] = Point(i, i);
          if (i >= 2) obstacles[obstacleCount++] = Point(GAME_WIDTH - 1 - i, i);
          obstacles[obstacleCount++] = Point(i, GAME_HEIGHT - 1 - i);
          obstacles[obstacleCount++] = Point(GAME_WIDTH - 1 - i, GAME_HEIGHT - 1 - i);
        }
        break;
        
      case 4:
        for (int y = 2; y < GAME_HEIGHT - 2; y++) {
          obstacles[obstacleCount++] = Point(GAME_WIDTH / 3, y);
          obstacles[obstacleCount++] = Point(2 * GAME_WIDTH / 3, y);
        }
        break;
        
      case 5:
        for (int x = 2; x < GAME_WIDTH - 2; x++) {
          obstacles[obstacleCount++] = Point(x, 2);
          obstacles[obstacleCount++] = Point(x, GAME_HEIGHT - 3);
        }
        for (int y = 2; y < GAME_HEIGHT - 2; y++) {
          obstacles[obstacleCount++] = Point(2, y);
          obstacles[obstacleCount++] = Point(GAME_WIDTH - 3, y);
        }
        break;
        
      case 6:
        for (int x = 4; x < GAME_WIDTH - 4; x++) {
          obstacles[obstacleCount++] = Point(x, GAME_HEIGHT / 2);
        }
        for (int y = 2; y < GAME_HEIGHT / 2; y++) {
          obstacles[obstacleCount++] = Point(GAME_WIDTH / 2, y);
        }
        break;
        
      case 7:
        for (int x = 3; x < GAME_WIDTH - 3; x += 2) {
          int y = (x % 4 == 0) ? GAME_HEIGHT / 3 : 2 * GAME_HEIGHT / 3;
          if (y >= 2) {
            for (int dy = -1; dy <= 1; dy++) {
              if (y + dy >= 2) {
                obstacles[obstacleCount++] = Point(x, y + dy);
              }
            }
          }
        }
        break;
        
      default:
        generateObstacles(2 + (lvl % 6));
        for (int i = 0; i < 5; i++) {
          int x = random(3, GAME_WIDTH - 3);
          int y = random(3, GAME_HEIGHT - 3); // Y >= 3 para evitar HUD
          obstacles[obstacleCount++] = Point(x, y);
        }
        break;
    }
  }

public:
  SnakeGame() : level(1), gameOver(false), isStarted(false) {}
  
  void init() {
    level = 1;
    gameOver = false;
    isStarted = false;
    
    // Iniciar serpientes fuera de la zona del HUD (Y >= 2)
    player1.init(5, GAME_HEIGHT/2 + 1, 1, 0);
    player2.init(GAME_WIDTH - 6, GAME_HEIGHT/2 + 1, -1, 0);
    
    generateObstacles(level);
    placeFood();
  }
  
  void start() { isStarted = true; }
  bool hasStarted() const { return isStarted; }
  bool hasEnded() const { return gameOver; }
  int getLevel() const { return level; }
  
  int getTotalScore() const { 
    return player1.score + player2.score; 
  }
  
  int getSpeed() const {
    int speed = BASE_SPEED - (level - 1) * 18;
    return speed < MIN_SPEED ? MIN_SPEED : speed;
  }
  
  void setPlayer1Direction(Direction dir) {
    switch(dir) {
      case DIR_UP_P1: player1.setDirection(0, -1); break;
      case DIR_DOWN_P1: player1.setDirection(0, 1); break;
      case DIR_LEFT_P1: player1.setDirection(-1, 0); break;
      case DIR_RIGHT_P1: player1.setDirection(1, 0); break;
    }
  }
  
  void setPlayer2Direction(Direction dir) {
    switch(dir) {
      case DIR_UP_P2: player2.setDirection(0, -1); break;
      case DIR_DOWN_P2: player2.setDirection(0, 1); break;
      case DIR_LEFT_P2: player2.setDirection(-1, 0); break;
      case DIR_RIGHT_P2: player2.setDirection(1, 0); break;
    }
  }
  
  bool update(bool &levelUp, bool &p1Ate, bool &p2Ate) {
    levelUp = false;
    p1Ate = false;
    p2Ate = false;
    
    if (!isStarted || gameOver) return false;
    
    if (player1.alive) player1.move();
    if (player2.alive) player2.move();
    
    // CORRECCIÓN 3: Verificar colisión con zona del HUD (Y < 2)
    if (player1.alive) {
      if (player1.body[0].x < 0 || player1.body[0].x >= GAME_WIDTH || 
          player1.body[0].y < 2 || player1.body[0].y >= GAME_HEIGHT) {
        player1.alive = false;
      }
    }
    
    if (player2.alive) {
      if (player2.body[0].x < 0 || player2.body[0].x >= GAME_WIDTH || 
          player2.body[0].y < 2 || player2.body[0].y >= GAME_HEIGHT) {
        player2.alive = false;
      }
    }
    
    // Check obstacle collisions
    for (int i = 0; i < obstacleCount; i++) {
      if (player1.alive && player1.body[0].equals(obstacles[i])) {
        player1.alive = false;
      }
      if (player2.alive && player2.body[0].equals(obstacles[i])) {
        player2.alive = false;
      }
    }
    
    // Check self collisions
    if (player1.alive && player1.checkSelfCollision()) {
      player1.alive = false;
    }
    if (player2.alive && player2.checkSelfCollision()) {
      player2.alive = false;
    }
    
    // Check collision between players
    if (player1.alive && player2.alive) {
      if (player1.body[0].equals(player2.body[0])) {
        player1.alive = false;
        player2.alive = false;
      } else {
        for (int i = 1; i < player2.length; i++) {
          if (player1.body[0].equals(player2.body[i])) {
            player1.alive = false;
            break;
          }
        }
        for (int i = 1; i < player1.length; i++) {
          if (player2.body[0].equals(player1.body[i])) {
            player2.alive = false;
            break;
          }
        }
      }
    }
    
    if (!player1.alive && !player2.alive) {
      gameOver = true;
      return false;
    }
    
    // Check food eating
    if (player1.alive && player1.body[0].equals(food)) {
      player1.grow();
      p1Ate = true;
      placeFood();
    }
    
    if (player2.alive && player2.body[0].equals(food)) {
      player2.grow();
      p2Ate = true;
      placeFood();
    }
    
    // Check level up
    int totalScore = getTotalScore();
    int newLevel = (totalScore / POINTS_PER_LEVEL) + 1;
    if (newLevel > level) {
      level = newLevel;
      levelUp = true;
      generateObstacles(level);
    }
    
    return p1Ate || p2Ate;
  }
  
  String toJSON() const {
    String json = "{\"type\":\"game\",";
    
    json += "\"snake1\":[";
    if (player1.alive) {
      for (int i = 0; i < player1.length; i++) {
        json += "[" + String(player1.body[i].x) + "," + String(player1.body[i].y) + "]";
        if (i < player1.length - 1) json += ",";
      }
    }
    json += "],";
    
    json += "\"snake2\":[";
    if (player2.alive) {
      for (int i = 0; i < player2.length; i++) {
        json += "[" + String(player2.body[i].x) + "," + String(player2.body[i].y) + "]";
        if (i < player2.length - 1) json += ",";
      }
    }
    json += "],";
    
    json += "\"obstacles\":[";
    for (int i = 0; i < obstacleCount; i++) {
      json += "[" + String(obstacles[i].x) + "," + String(obstacles[i].y) + "]";
      if (i < obstacleCount - 1) json += ",";
    }
    json += "],";
    
    json += "\"food\":[" + String(food.x) + "," + String(food.y) + "],";
    json += "\"score1\":" + String(player1.score) + ",";
    json += "\"score2\":" + String(player2.score) + ",";
    json += "\"alive1\":" + String(player1.alive ? "true" : "false") + ",";
    json += "\"alive2\":" + String(player2.alive ? "true" : "false") + ",";
    json += "\"level\":" + String(level) + ",";
    json += "\"gameOver\":" + String(gameOver ? "true" : "false") + "}";
    
    return json;
  }
};

#endif
