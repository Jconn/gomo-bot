#ifndef SPOT_H
#define SPOT_H 

#include "MoveType.h"
#include "Coordinate.h"
struct spot{
  MoveType owner;
  Coordinate topLeft;
  Coordinate topRight;
  Coordinate botLeft;
  Coordinate botRight;
};

#endif
