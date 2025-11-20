#ifndef POINT_H
#define POINT_H

struct Point {
  int x;
  int y;
  
  Point() : x(0), y(0) {}
  Point(int _x, int _y) : x(_x), y(_y) {}
  
  bool equals(const Point& other) const {
    return x == other.x && y == other.y;
  }
};

enum Direction {
  // Jugador 1
  DIR_UP_P1 = 1,
  DIR_DOWN_P1 = 2,
  DIR_LEFT_P1 = 3,
  DIR_RIGHT_P1 = 4,
  
  // Jugador 2
  DIR_UP_P2 = 11,
  DIR_DOWN_P2 = 12,
  DIR_LEFT_P2 = 13,
  DIR_RIGHT_P2 = 14,
  
  DIR_NONE = 0
};

#endif
