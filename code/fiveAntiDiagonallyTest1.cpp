#include <iostream>
#include "Gomoku.h"
#include "Coordinate.h"
#include "MoveType.h"

int main() {
  Gomoku game;
  
  int row = 13;
  int col = 13;
  
  Coordinate mostRecentMove(row,col);

  std::cout << "most recent move located at row = " << row << ", col = " << col << '\n';
  
  game.setMove(17, 17, black);
  game.setMove(16, 16, black);
  game.setMove(15, 15, black);
  game.setMove(14, 14, black);
  game.setMove(13, 13, black);

  game.setMove(14, 7, white);
  game.setMove(14, 8, white);
  game.setMove(8, 13, white);

  game.printGameState();

  if(game.fiveAntiDiagonally(mostRecentMove, black)) {
    std::cout << "a player wins!\n"; // TODO: update this 
  } else {
    std::cout << "no win detected\n";
  }
}


