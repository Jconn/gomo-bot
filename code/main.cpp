#include <iostream> // cout, cin
#include <vector>   // vector
#include "Gomoku.h"
#include "Coordinate.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


using namespace cv;
using namespace std;


int main(int argc, char** argv)
{
  cv::CommandLineParser parser(argc, argv,
      "{help h ||}{@image|../data/board.jpg|}"
      );
  string filename = parser.get<string>("@image");
  if (filename.empty())
  {
    cout << "no image_name provided" << endl;
    return -1;
  }  
  Gomoku game;
  std::vector<Coordinate> unoccupied_spaces;
  game.populateBoard(filename);
  return 0;
  // initialize the vector
  for(int i = 0; i < GRID_LENGTH; ++i) {
    for(int j = 0; j < GRID_LENGTH; ++j) {
      Coordinate a(i,j);
      unoccupied_spaces.push_back(a);
    }
  }

  Coordinate mostRecentMove(0,0);

  while(!game.getGameEnded() && !game.isDraw()) {
    mostRecentMove = game.getAIMove();
    game.printGameState();


    if(game.winningMove(mostRecentMove)) {
      std::cout << "a computer wins!\n"; // TODO: update this
      game.setGameEnded();
      break;
    }
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

    // check if game is won 

    // check if game is a draw
    if(game.isDraw())
      game.setGameEnded();

  }

  std::cout << "program ending\n";
  return 0;
}

