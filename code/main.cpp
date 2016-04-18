#include <iostream> // cout, cin
#include <vector>   // vector
#include "Gomoku.h"
#include "Coordinate.h"
#include "MoveType.h"
#include "GameMode.h"
//#include "perspectivetransform.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include "observepieces.h"
using namespace cv;
using namespace std;

int main(int argc, char** argv) {

  cv::CommandLineParser parser(argc, argv,
      "{help h ||}{@image||}"
      );
  string filename = parser.get<string>("@image");

  if (!filename.empty())
  {
    cout << "in test mode " << endl;  
    cout << "filename is  " << filename << endl;
    observePieces(filename,-1);  
  }

  // initialize the vector with empty coordinates
  std::vector<Coordinate> unoccupied_spaces;
  for(int i = 0; i < GRID_LENGTH; ++i) {
    for(int j = 0; j < GRID_LENGTH; ++j) {
      Coordinate a(i,j);
      unoccupied_spaces.push_back(a);
    }
  }

  Coordinate mostRecentMove(0,0); // initialize mostRecentMove
  cout << " creating game " << endl;

  cout << "Select game mode, one of two player, three player, or four player.\n"
    << "  Two player: 1 human, 1 AI player.\n"
    << "Three player: 2 human, 1 AI player.\n"
    << " Four player: 3 human, 1 AI player.\n";


  cout << "Type \"2\", \"3\", or \"4\"\n";

  char game_mode = '\0';

  cin >> game_mode;
  int num_players = game_mode - '0';

  num_players++;
  Gomoku game(game_mode);

  cout << "entering game loop " << endl;

  while(!game.isEnded() && !game.isDraw()) {
    cout << " populating the the gamestate " << endl;  

    mostRecentMove = game.getAIMove();
    game.printGameState();

    if(game.winningMove(mostRecentMove)) {
      std::cout << "a computer wins!\n"; // TODO: update this
   		 
			Mat lose;			
      if( !(lose=imread("lose.jpg", CV_LOAD_IMAGE_COLOR)).data)
        return 0;
    	imshow("You lose!", lose);
    
			game.setGameEnded();
      break;
    }

    Coordinate enemyMove;	

    // BEGIN CODE: PLAY AI ON PHYSICAL BOARD ///////////////////////////////////////////
    /*
    bool spotted = false;
    while(!spotted){
    cout << " move please (press 0 when done) " << endl;
    int doneVal = -1;	
    while(doneVal!=0){
    cin >> doneVal;
    }
    system("raspistill -t 1000 -n -o move.jpg");
    cout << " picture has been taken" << endl;


    // perform perspective transform
    //			perspectiveTransform("move.jpg");

    spotted = game.populateBoard("move.jpg", enemyMove);
    if(!spotted)
    cout << "did not detect picture taken " << endl;	
    }
    */
    // END CODE: PLAY AI ON PHYSICAL BOARD //////////////////////////////////////////////

    // BEGIN CODE: PLAY AI IN TERMINAL ///////////////////////////////////////////
    for(unsigned int i = 0; i < num_players - 2; ++i) {
      mostRecentMove = game.getHumanMove(unoccupied_spaces, i);
      game.printGameState();
      if(game.winningMove(mostRecentMove)) {
        std::cout << "a player wins!\n"; // TODO: update this
        game.setGameEnded();
        break;
      }
    }
    // END CODE: PLAY AI IN TERMINAL ////////////////////////////////////////////

    game.printGameState();

    // check if game is won 
    if(game.winningMove(enemyMove)) {
      std::cout << "a player wins!\n"; // TODO: update this
			Mat win;			
      if( !(win=imread("win.jpg", CV_LOAD_IMAGE_COLOR)).data)
        return 0;
    	imshow("You win!", win);
      
			game.setGameEnded();
      break;
    }

    // check if game is ended 
    if(game.isEnded())
      game.setGameEnded();

    // check if game is a draw
    if(game.isDraw())
      game.setGameEnded();

  }

  std::cout << "program ending\n";
  return 0;
}

