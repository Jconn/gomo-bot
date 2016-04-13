#ifndef THREAT_MOVE_H
#define THREAT_MOVE_H


#include "Coordinate.h"

struct potentialMove{
  Coordinate move;
  bool isWinning;
  int depth;

};


#endif
