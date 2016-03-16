#include "Gomoku.h"
<<<<<<< Updated upstream
#include <iostream>
=======
#include <iostream> // cout, cin

>>>>>>> Stashed changes
int main() {
  std::cout << "everyone suffers\n";
  
  Gomoku game;



  //TODO: handle human vs. AI going first
  //TODO: handle human assignment to black or white

  while(!game.getGameEnded() && !game.isDraw()) {
    // get the move of human opponent
    game.getHumanMove();



    // determine the move of    

    // check if game is a draw
    if(game.isDraw())
      game.setGameEnded(true);
  }


  return 0;
}

