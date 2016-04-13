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

const int NUM_OPENING_PLAYBOOK_MOVES = 9;

using namespace std;
using namespace cv;

struct compositeCircle{
	Vec3i circle;
	int numCombines; // number of times we combine circles that are too close together
	MoveType color;
};

enum AI{randomAI,defensive,offensive}; 

class Gomoku{
	// game meta-data
	int numMovesPlayed;
	bool winnerDetermined;
	bool gameEnded; 	// 2 ways for gameEnded to be true:
						// (1) before all grid squares are filled, one player gets 5 in a row
						// (2) all grid squares filled without a player getting 5 in a row

	// internal representation of board
	Spot board[GRID_LENGTH][GRID_LENGTH]; // (0,0) corresponds to the top left of the board
	
	vector<Coordinate> myMoves;
	vector<Coordinate> enemyMoves;
	vector<Coordinate> openingPlaybook;
	
	AI myMode = defensive; 
	void setAIMove(Coordinate &coord); 

	public:
	
	// getter-setter pairs for private members
	int getNumMovesPlayed() const {return numMovesPlayed;}
	void incrementNumMovesPlayed() {numMovesPlayed++;}
	
	bool getWinnerDetermined() const {return winnerDetermined;}
	void setWinnerDetermined() {winnerDetermined = true;}

	bool isEnded() const {return gameEnded;}
	void setGameEnded() {gameEnded = true;}

	// constructor
	Gomoku();
	void initializePlaybook();

	Coordinate getAIMove();
	Coordinate getRandomAIMove(vector<Coordinate>& us); // TODO: merge this


	Coordinate getAttack();
	Coordinate getDefense();

	bool populateBoard(string filename, Coordinate &enemyMove); //

	Coordinate getHumanMove(vector<Coordinate>& us);	
	

	bool isDraw();
	bool isBlank(Coordinate location); 

	// function determining winning move & its helpers
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

	// testing functions
	void setMove(int x, int y, MoveType type);
	void printGameState();
};

#endif // GOMOKU_H
