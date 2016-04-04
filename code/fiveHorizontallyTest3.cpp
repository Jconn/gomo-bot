#include <iostream>
#include "Gomoku.h"
#include "Coordinate.h"
#include "MoveType.h"

int main() {
  Gomoku game;
  
  int row = 0;
  int col = 15;
  
  Coordinate mostRecentMove(row,col);

  std::cout << "most recent move located at row = " << row << ", col = " << col << '\n';

  game.setMove(0, 13, black);
  game.setMove(0, 14, black);
  game.setMove(0, 15, black);
  game.setMove(0, 16, black);
  game.setMove(0, 17, black);

  game.printGameState();

  if(game.fiveHorizontally(mostRecentMove, black)) {
    std::cout << "a player wins!\n"; // TODO: update this 
  } else {
    std::cout << "no win detected\n";
  }
}



