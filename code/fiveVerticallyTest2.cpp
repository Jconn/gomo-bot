#include <iostream>
#include "Gomoku.h"
#include "Coordinate.h"
#include "MoveType.h"

int main() {
  Gomoku game;
  
  int row = 17;
  int col = 0;
  
  Coordinate mostRecentMove(row,col);

  std::cout << "most recent move located at row = " << row << ", col = " << col << '\n';
  
  game.setMove(0, 0, black);
  game.setMove(1, 0, black);
  game.setMove(2, 0, black);
  game.setMove(3, 0, black);
  game.setMove(4, 0, black);
  game.setMove(5, 0, white);

  game.setMove(1, 0, white);
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

