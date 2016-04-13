#ifndef GOMOKU_H
#define GOMOKU_H

#include <iostream>      // cin, cout
#include <time.h>        // time
#include <stdlib.h>      // srand, rand 
#include <cassert>       // assert
#include <vector>        // vector
#include <algorithm>     // remove
#include <string>
#include "Coordinate.h"
#include "Constants.h"
#include "MoveType.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "threats.h"
#include "Spot.h"

#include "threatMove.h"


using namespace std;
using namespace cv;

struct compositeCircle{
	Vec3i circle;
	int numCombines;
	MoveType color;
};

enum AI{randomAI,defensive,intelligent}; 

class Gomoku{
	MoveType myColor;
	int numMovesPlayed;
	bool winnerDetermined;
	spot board[GRID_LENGTH][GRID_LENGTH]; // (0,0) corresponds to the top left of the board

  //vector that stores all the moves the computer has made
  vector <Coordinate> myMoves;

  //vector that stores all moves the human player has made
  //TODO: extend this to multiple human players, for multi-player
  vector <Coordinate> enemyMoves;

  //vector that stores the initial moves the computer considers
  vector <Coordinate> openingPlaybook;

  // 2 ways for gameEnded to be true:
	// (1) before all grid squares are filled, one player gets 5 in a row
	// (2) all grid squares filled without a player getting 5 in a row
	bool gameEnded; 
	bool withinRegion( Coordinate point, spot curSpot);
	AI myMode = intelligent; 
	void setAIMove(Coordinate &coord); 

	public:

	Gomoku();
  

  //////////////////////////////////////////////////////////////////////////////////
  // start AI functions
  //////////////////////////////////////////////////////////////////////////////////
  
  //general function to be called by main, returns coordinate that the AI decided to move to
  Coordinate getAIMove();

  //function that gets the defensive mode move
  potentialMove getDefense();

  //function that gets the intelligent mode move
  potentialMove getAttack();
  
  //function that gets the random mode move
  Coordinate getRandomAIMove(vector<Coordinate>& us);

  //creates a list of moves for the ai to try at the beginning of the game
  void initializePlaybook();

  //////////////////////////////////////////////////////////////////////////////////
  // end AI functions
  //////////////////////////////////////////////////////////////////////////////////



  bool getWinnerDetermined() const {return winnerDetermined;}
  void setWinnerDetermined() {winnerDetermined = true;}
  Coordinate getHumanMove(vector<Coordinate>& us);
  //void observeBoard(int (*gameState)[GRID_LENGTH][GRID_LENGTH]);    
  int getNumMovesPlayed() const {return numMovesPlayed;}
  void incrementNumMovesPlayed() {numMovesPlayed++;}
  bool isEnded() const {return gameEnded;} // getter paired w/ setGameEnded
  void setGameEnded() {gameEnded = true;}
  bool isDraw();

  bool isFree(Coordinate location); 
  
  //////////////////////////////////////////////////////////////////////////////////
  // start image processing functions
  //////////////////////////////////////////////////////////////////////////////////


  //this function checks to see how many pieces are currently placed on the board, and 
  //updates the game state if we detect one new piece from each human player
  bool populateBoard(string filename, Coordinate &enemyMove);

  
  //this function populates the pixel location of each spot on the board
  //using either a stock image or a picture taken at the beginning of the game
  void observeBoard(int (*gameState)[GRID_LENGTH][GRID_LENGTH]);   

  //////////////////////////////////////////////////////////////////////////////////
  // end image processing functions
  //////////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////////////
  // start winningMove functions
  //////////////////////////////////////////////////////////////////////////////////
	bool winningMove(Coordinate most_recent_move);

	bool fiveHorizontally(Coordinate coord);
	void checkLeftOfMove(Coordinate coord, int& num_tiles_in_a_row);
	void checkRightOfMove(Coordinate coord, int& num_tiles_in_a_row);

	bool fiveVertically(Coordinate coord);
	void checkAboveMove(Coordinate coord, int& num_tiles_in_a_row);
	void checkBelowMove(Coordinate coord, int& num_tiles_in_a_row);

	bool fiveDiagonally(Coordinate coord);
	void checkAboveAndRightOfMove(Coordinate coord, int& num_tiles_in_a_row);
	void checkBelowAndLeftOfMove(Coordinate coord, int& num_tiles_in_a_row);

	bool fiveAntiDiagonally(Coordinate coord);
	void checkAboveAndLeftOfMove(Coordinate coord, int& num_tiles_in_a_row);
	void checkBelowAndRightOfMove(Coordinate coord, int& num_tiles_in_a_row);

	//////////////////////////////////////////////////////////////////////////////////
	// end winningMove functions
	//////////////////////////////////////////////////////////////////////////////////

	// testing functions
	void setMove(int x, int y, MoveType type);
	void printGameState();
};

#endif // GOMOKU_H
