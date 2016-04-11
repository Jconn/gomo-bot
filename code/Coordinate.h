#ifndef COORDINATE_H
#define COORDINATE_H
#include "Constants.h"
#include <cassert>
#include <iostream>
enum Direction{diag=0,lr=1,ud=2,antidiag=3};
struct Coordinate {
    int x;
    int y;
    
    bool isValid();

    Coordinate(int x, int y);
    Coordinate();
    bool move(Direction moveDir, int numMoves,int mag);
    bool decrement(Direction moveDir, int numMoves);
    bool increment(Direction moveDir, int numMoves);
    int movesFrom(Direction moveDir, Coordinate otherLoc);

};

inline bool operator==(const Coordinate&  lhs, const Coordinate& rhs){

  return ((lhs.x==rhs.x) && (lhs.y ==rhs.y));
}

inline bool operator!=(const Coordinate&  lhs, const Coordinate& rhs){
  return( !(lhs==rhs));
}


#endif // COORDINATE_H
