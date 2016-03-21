#ifndef COORDINATE_H
#define COORDINATE_H
#include "Constants.h"


struct Coordinate {
    int x;
    int y;
    
    bool isValid();

    Coordinate(int x, int y);
};

Coordinate::Coordinate(int a, int b) {
  x = a;
  y = b;
}

bool Coordinate::isValid() {
  if(x > GRID_LENGTH || y > GRID_LENGTH || x < 0 || y < 0)
    return false;
  else
    return true;
}

#endif // COORDINATE_H
