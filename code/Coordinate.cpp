#include "Coordinate.h"
#include "Constants.h"
using namespace std;
Coordinate::Coordinate(int a, int b) {
  x = a;
  y = b;
}
Coordinate::Coordinate() {
  x = -1; 
  y = -1; 
}


bool Coordinate::isValid() {
  if(x > GRID_LENGTH || y > GRID_LENGTH || x < 0 || y < 0)
    return false;
  else
    return true;
}


bool Coordinate::move(Direction moveDir, int numMoves,int mag){
  bool wasValid = true;
  //cout << "moving point...\n";
  //cout << "original point is " << x <<"," << y << endl; 
  if(mag!=-1 || mag!=1)
    assert(1);
  if(!isValid())
    assert(1);
  switch(moveDir){
    case diag:
      while(isValid() && numMoves){
        x+=mag;
        y-=mag;
        numMoves--;
      }
      while(!isValid()){
        x-=mag;
        y+=mag;
        wasValid = false;
      }
      break;
    case ud:
      while(isValid() && numMoves){
        y-=mag;
        numMoves--;
      }
      while(!isValid()){
        y+=mag;
        wasValid = false;
      }
      break;
    case lr:
      while(isValid() && numMoves){
        x+=mag;
        numMoves--;
      }
      while(!isValid()){
        x-=mag;
        wasValid = false;
      }
      break;
    case antidiag:
      while(isValid() && numMoves){
        x+=mag;
        y+=mag;
        numMoves--;
      }
      while(!isValid()){
        x-=mag;
        y-=mag;
        wasValid = false;
      }
      break;
  }
  //cout << " new point is: " << x << "," << y << endl; 
  //cout << "point valid param: " << wasValid << endl; 
  return wasValid;
}

bool Coordinate::increment(Direction moveDir, int numMoves){
  return move(moveDir,numMoves,1);
}
bool Coordinate::decrement(Direction moveDir, int numMoves){
  return move(moveDir,numMoves,-1);
}
int Coordinate::movesFrom(Direction moveDir, Coordinate otherLoc){
  Coordinate iterator1 = otherLoc;
  int moves = 0;
  cout << "comparing coords: " << this->x << "," << this->y << " and " << otherLoc.x << "," <<otherLoc.y << endl;
  while(iterator1 != *this){
    if(!iterator1.increment(moveDir,1)){
      moves = -1; 
      break; 
    }
    ++moves; 
  }
  if(moves >=0){
    cout << "locations are " << moves << " moves apart" << endl; 
    return moves;
  }
  iterator1 = otherLoc;
  moves = 0;
  while(iterator1 != *this){
    if(!iterator1.decrement(moveDir,1)){
      return -1; 
    }
    ++moves; 
  }
  cout << "locations are " << moves << " moves apart" << endl;
  return moves;
}
