#include <iostream>
#include "Gomoku.h"
#include "Coordinate.h"
#include "MoveType.h"

int main() {
  Gomoku game;
  
  int row = 7;
  int col = 7;
  
  Coordinate mostRecentMove(row,col);

  std::cout << "most recent move located at row = " << row << ", col = " << col << '\n';
  
  game.setMove(7, 7, black);
  game.setMove(8, 7, black);
  game.setMove(9, 7, black);
  game.setMove(10, 7, black);
  game.setMove(11, 7, black);
  game.setMove(5, 7, white);

  game.setMove(1, 1, white);
  game.setMove(1, 3, white);
  game.setMove(1, 4, white);

  game.printGameState();

  if(game.fiveVertically(mostRecentMove, black)) {
    std::cout << "a player wins!\n"; // TODO: update this 
  } else {
    std::cout << "no win detected\n";
  }
}

