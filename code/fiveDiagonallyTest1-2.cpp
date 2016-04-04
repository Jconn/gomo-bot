#include <iostream>
#include "Gomoku.h"
#include "Coordinate.h"
#include "MoveType.h"

int main() {
  Gomoku game;
  
  int row = 10;
  int col = 11;
  
  Coordinate mostRecentMove(row,col);

  std::cout << "most recent move located at row = " << row << ", col = " << col << '\n';
  
  game.setMove(9, 12, black);
  game.setMove(10, 11, black);
  game.setMove(11, 10, black);
  game.setMove(12, 9, black);
  game.setMove(13, 8, black);

  game.setMove(14, 7, white);
  game.setMove(14, 8, white);
  game.setMove(8, 13, white);

  game.printGameState();

  if(game.fiveDiagonally(mostRecentMove, black)) {
    std::cout << "a player wins!\n"; // TODO: update this 
  } else {
    std::cout << "no win detected\n";
  }
}


