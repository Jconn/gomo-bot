#include <iostream> // cout, cin
#include <vector>   // vector
#include "Gomoku.h"
#include "Coordinate.h"

int main() {
  
  Gomoku game;
  std::vector<Coordinate> unoccupied_spaces;

  // initialize the vector
  for(int i = 0; i < GRID_LENGTH; ++i) {
    for(int j = 0; j < GRID_LENGTH; ++j) {
      Coordinate a(i,j);
      unoccupied_spaces.push_back(a);
    }
  }

  Coordinate mostRecentMove(0,0);

  while(!game.getGameEnded() && !game.isDraw()) {

    mostRecentMove = game.getHumanMove(unoccupied_spaces);
    game.printGameState();
  
    // check if game is won 
    if(game.winningMove(mostRecentMove)) {
      std::cout << "a player wins!\n"; // TODO: update this
      game.setGameEnded();
      break;
    }
    
    // check if game is a draw
    if(game.isDraw())
      game.setGameEnded();

    mostRecentMove = game.getRandomAIMove(unoccupied_spaces);
    // check if game is won 
    if(game.winningMove(mostRecentMove)) {
      std::cout << "a player wins!\n"; // TODO: update this
      game.setGameEnded();
      break;
    }
    // check if game is a draw
    if(game.isDraw())
      game.setGameEnded();

  game.printGameState();
   
  }

  std::cout << "program ending\n";
  return 0;
}
