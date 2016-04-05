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
using namespace std;
using namespace cv;

struct compositeCircle{
  Vec3i circle;
  int numCombines;
  MoveType color;
};

enum AI{randomAI,defensive,offensive}; 

class Gomoku{
  MoveType myColor;
  int numMovesPlayed;
  bool winnerDetermined;
  spot board[GRID_LENGTH][GRID_LENGTH]; // (0,0) corresponds to the top left of the board
  vector <Coordinate> myMoves;
  vector <Coordinate> openingPlaybook;
  vector <Coordinate> enemyMoves;
  // 2 ways to be true:
  // (1) before all grid squares are filled, one player gets 5 in a row
  // (2) all grid squares filled without a player getting 5 in a row
  bool gameEnded; 
  bool withinRegion( Coordinate point, spot curSpot);
  AI myMode = defensive; 
  void setAIMove(Coordinate &coord); 
  public:
  Gomoku();
  Coordinate getAIMove();
  Coordinate getDefense();
  void observeBoard(int (*gameState)[GRID_LENGTH][GRID_LENGTH]);    
  void populateBoard(string filename);
  bool getWinnerDetermined() const {return winnerDetermined;}
  void setWinnerDetermined() {winnerDetermined = true;}
  Coordinate getHumanMove(vector<Coordinate>& us);
  //void observeBoard(int (*gameState)[GRID_LENGTH][GRID_LENGTH]);    
  int getNumMovesPlayed() const {return numMovesPlayed;}
  void incrementNumMovesPlayed() {numMovesPlayed++;}
  Coordinate getAttack();
  bool getGameEnded() const {return gameEnded;}
  void setGameEnded() {gameEnded = true;}
  bool isDraw();
  Coordinate getRandomAIMove(vector<Coordinate>& us);
  void populatePlaybook();
  bool isFree(Coordinate location); 
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
