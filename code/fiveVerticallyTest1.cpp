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
  
  game.setMove(12, 0, white);
  game.setMove(13, 0, black);
  game.setMove(14, 0, black);
  game.setMove(15, 0, black);
  game.setMove(16, 0, black);
  game.setMove(17, 0, black);

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
