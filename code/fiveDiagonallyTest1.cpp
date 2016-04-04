#include <iostream>
#include "Gomoku.h"
#include "Coordinate.h"
#include "MoveType.h"

int main() {
  Gomoku game;
  
  int row = 13;
  int col = 4;
  
  Coordinate mostRecentMove(row,col);

  std::cout << "most recent move located at row = " << row << ", col = " << col << '\n';
  
  game.setMove(13, 4, black);
  game.setMove(14, 3, black);
  game.setMove(15, 2, black);
  game.setMove(16, 1, black);
  game.setMove(17, 0, black);

  game.setMove(1, 1, white);
  game.setMove(1, 3, white);
  game.setMove(1, 4, white);

  game.printGameState();

  if(game.fiveDiagonally(mostRecentMove, black)) {
    std::cout << "a player wins!\n"; // TODO: update this 
  } else {
    std::cout << "no win detected\n";
  }
}


