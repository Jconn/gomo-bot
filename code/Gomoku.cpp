
#include <iostream>      // cin, cout
#include <time.h>        // time
#include <stdlib.h>      // srand, rand 
#include <cassert>       // assert
#include <vector>        // vector
#include <algorithm>     // remove
#include "Coordinate.h"
#include "Constants.h"
#include "MoveType.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "threats.h"
#include "grid.h"
#include "observepieces.h"
using namespace std;
using namespace cv;
#include <unistd.h>
#include <fstream>


Gomoku::Gomoku() {
	// initialize all board values to blank
	for(int i = 0; i < GRID_LENGTH; i++) {
		for (int j = 0; j < GRID_LENGTH; j++) {
			board[i][j].owner = blank;
		}
	}
	numMovesPlayed = 0;
	gameEnded = false;
	//system("raspistill -t 1000 -o empty.jpg");	
	if(!populateGridEdges("../images/pierpont/connector/001.jpg",board))
		assert(0);
	populatePlaybook();
}

Coordinate Gomoku::getHumanMove(vector<Coordinate>& us) {
	Coordinate coord(0,0);

	bool validMove = false;
	while(!validMove){
		std::cout << "Where would you like to move? Enter the horizontal coordinate in [1,18].\n";
		std::cin >> coord.y;

		std::cout << "Where would you like to move? Enter the vertical coordinate in [1,18].\n";
		std::cin >> coord.x;

		us.pop_back();

		us.erase(std::remove(us.begin(), us.end(), coord), us.end());

		MoveType proposedMoveType = board[coord.x][coord.y].owner;

		if(proposedMoveType == blank){
			board[coord.x][coord.y].owner = PLAYER_COLOR;
			validMove = true;
		}
		else {
			std::cout << "error, this space already occupied sux 4 u\n";
		}
	}
	enemyMoves.push_back(coord); 
	incrementNumMovesPlayed();

	return coord;
}

Coordinate Gomoku::getAIMove(){
	cout << " grabbing ai move " << endl;	
	Coordinate curMove;
	if(myMode ==randomAI){
		assert(1); 
		//myMoves.push_back(getRandomAIMove(NULL));
		curMove = myMoves[myMoves.size()-1];
	}
	if(myMode == defensive){
		myMoves.push_back(getDefense());
		curMove = myMoves[myMoves.size()-1];
	}
	if(myMode == offensive){
		myMoves.push_back(getAttack());
		curMove = myMoves[myMoves.size()-1]; 
	}
	curMove = myMoves[myMoves.size()-1];
	setAIMove(curMove);
	return curMove;
}

void Gomoku::populatePlaybook(){
	openingPlaybook.push_back(Coordinate(9,9));
	openingPlaybook.push_back(Coordinate(10,10));
	openingPlaybook.push_back(Coordinate(8,8));
	openingPlaybook.push_back(Coordinate(8,10));
	openingPlaybook.push_back(Coordinate(10,8));
	openingPlaybook.push_back(Coordinate(9,8));
	openingPlaybook.push_back(Coordinate(8,9));
	openingPlaybook.push_back(Coordinate(10,9));
	openingPlaybook.push_back(Coordinate(10,9));
}

Coordinate Gomoku::getRandomAIMove(vector<Coordinate>& us) {
	Coordinate coord(0,0);

	int rand_vec_idx = 0;

	srand(time(NULL));

	while(1) {
		rand_vec_idx = rand() % us.size(); 

		coord = us[rand_vec_idx];

		us.erase(std::remove(us.begin(), us.end(), coord), us.end());


		if(board[coord.x][coord.y].owner == blank) {
			std::cout << "random AI moves to " << coord.x << ", " << coord.y << "\n";
			board[coord.x][coord.y].owner = AI_COLOR;
			break;
		}
	}
	incrementNumMovesPlayed();

	return coord;
}

void Gomoku::setAIMove(Coordinate &coord){


	if(board[coord.x][coord.y].owner == blank) {
		std::cout << "AI moves to " << coord.x << ", " << coord.y << "\n";
		board[coord.x][coord.y].owner = AI_COLOR;
	}
	incrementNumMovesPlayed();
}
Coordinate Gomoku::getDefense(){
	//we have the board, loop through our spaces, and find threats that we have,
	//but at the start of our play, just follow the playbook
	cout << "enemyMoves size is " << enemyMoves.size() << endl;
	if(myMoves.size() < 2){
		cout << "going into opening playbook" << endl; 
		for(unsigned int i = 0; i < openingPlaybook.size(); ++i){
			if(isFree(openingPlaybook[i])){
				cout << "moving to " << openingPlaybook[i].x <<"," << openingPlaybook[i].y << endl; 
				return openingPlaybook[i]; 
			}
		}
	}
	vector <Threat> threats;

	cout << "finding initial threats.. " << endl;
	for(unsigned int i = 0; i < enemyMoves.size(); ++i){ 
		findInitThreats(enemyMoves[i],threats,board,PLAYER_COLOR);
	}
	for(int i = 0; i < static_cast<int>(threats.size()); ++i){
		if(threats[i].winningThreat){
			return threats[i].gainSquare;
		}
	}
	Coordinate bestDef;
	//TODO: replace findBestMove with findBestDefense
	if( threats.size() > 0){ 
		if(findBestDefense(threats,bestDef))
			return bestDef;
		else
			return getAttack();
	}
	return getAttack();
}

Coordinate Gomoku::getAttack(){
	//we have the board, loop through our spaces, and find threats that we have,
	//but at the start of our play, just follow the playbook
	cout << "myMoves size is " << myMoves.size() << endl;
	if(myMoves.size() < 3){
		cout << "going into opening playbook" << endl; 
		for(unsigned int i = 0; i < openingPlaybook.size(); ++i){
			if(isFree(openingPlaybook[i])){
				cout << "moving to " << openingPlaybook[i].x <<"," << openingPlaybook[i].y << endl; 
				return openingPlaybook[i]; 
			}
		}
	}
	vector <Threat> threats;  
	cout << "finding initial threats.. " << endl;
	for(unsigned int i = 0; i < myMoves.size(); ++i){ 
		findInitThreats(myMoves[i],threats,board,AI_COLOR);
	}
	for(int i = 0; i < static_cast<int>(threats.size()); ++i){
		if(threats[i].winningThreat){
			return threats[i].gainSquare;
		}
	}


	//now have the initial threats;
	vector <Threat*> threatPtrs;
	for(unsigned int i = 0; i < threats.size(); ++i){
		threatPtrs.push_back(&(threats[i]));
	}
	cout <<"detected " << threats.size() << " initial threats " << endl;
	cout << "recursively finding child threats.. " << endl;
	for(unsigned int i = 0; i < threats.size(); ++i){
		threats[i].printThreat(); 
		threats[i].findChildThreats(10); 
	}
	cout << " finished child threats " << endl;
	int bestDepth = -1;
	int bestIndex = -1;
	for(int i = 0; i < static_cast<int>(threats.size()); ++i){
		if(threats[i].winningThreat && threats[i].winningDepth > bestDepth){
			bestIndex = i;
			bestDepth = threats[i].winningDepth;
		}
	}
	if(bestIndex >=0)
		return threats[bestIndex].gainSquare;


	if( threats.size() > 0) 
		return findBestMove(threats);
	//we did not find a winning sequence, we did not even find a threat
	//time to get diry
	//TODO: stop being dirty
	for(int i =0; i <GRID_LENGTH; ++i){
		for(int j = 0; j < GRID_LENGTH; ++j){
			if(board[i][j].owner==blank){
				Threat newThreat(i,j,board);
				threats.push_back(newThreat);
			}

		}
	}
	cout << "attempting to find best move among shittters " << endl;
	cout << "threat size is " <<threats.size() << endl;
	for(unsigned int i = 0; i < threats.size(); ++i){
		threats[i].findChildThreats(5); 
	}


	if( threats.size() > 0) 
		return findBestMove(threats);

	if(myMoves.size() < 3){
		cout << "going into opening playbook" << endl; 
		for(unsigned int i = 0; i < openingPlaybook.size(); ++i){
			if(isFree(openingPlaybook[i])){
				cout << "moving to " << openingPlaybook[i].x <<"," << openingPlaybook[i].y << endl; 
				return openingPlaybook[i]; 
			}
		}
	}
	assert(0);
	//TODO: fix this return
	return openingPlaybook[0];
	//we have completed our opening moves, now determine what threats we have
}



vector <Threat> getThreats(){
	//loop through our moves, call each move a "threat"
	//and then after each move is called a "threat"
	//determine their children, these are our actual initial threats
	//parse all the children together, and return a vector of these coordinates
	//TODO: USE THIS SHIT 
	vector <Threat> totalThreats; 
	// for(int i = 0; i < myMoves.size(); ++i){
	//  Threat curMove(myColor,); 
	// }
	return totalThreats;
}




bool Gomoku::isDraw() {
	if(numMovesPlayed == GRID_LENGTH * GRID_LENGTH && !winnerDetermined)
		return true;
	else
		return false;
}

bool Gomoku::winningMove(Coordinate most_recent_move) {
	if(fiveHorizontally(most_recent_move)      || fiveVertically(most_recent_move)
			|| fiveDiagonally(most_recent_move)     || fiveAntiDiagonally(most_recent_move))
		return true;
	else
		return false;
}

bool Gomoku::fiveHorizontally(Coordinate coord) {
	int num_tiles_in_a_row = 1;  // if this == 5, the game is won!

	checkLeftOfMove(coord, num_tiles_in_a_row);

	checkRightOfMove(coord, num_tiles_in_a_row);

	if(num_tiles_in_a_row == 5)
		return true;
	else
		return false;
}

void Gomoku::checkLeftOfMove(Coordinate coord, int& num_tiles_in_a_row) {
	MoveType type = board[coord.x][coord.y].owner;
	Coordinate current_coord = coord;           // coordinate of the grid square under current inspection 
	int num_tiles_inspected = 0;                // highest possible value is 4     

	// step (1): check to the left of most recent move 
	current_coord.y = coord.y - 1;  // TODO: adjust for each function
	MoveType current_coord_move_type = board[current_coord.x][current_coord.y].owner; 

	while(current_coord.isValid() && current_coord_move_type == type && num_tiles_inspected <= MAX_TILES_PER_DIRECTION) {

		// after we've made sure the next coordinate is valid, we can safely index to that space
		num_tiles_in_a_row++;
		current_coord.y--;            // TODO: adjust for each function
		num_tiles_inspected++;
		current_coord_move_type = board[current_coord.x][current_coord.y].owner; 
	}
}

void Gomoku::checkRightOfMove(Coordinate coord, int& num_tiles_in_a_row) {
	MoveType type = board[coord.x][coord.y].owner;
	Coordinate current_coord = coord;           // grid squares we are inspecting 
	int num_tiles_inspected = 0;

	// step (2): check to the right of the most recent move
	//     note: need to add an extra condition, num_same_tiles <= 4
	current_coord.y = coord.y + 1; 
	MoveType current_coord_move_type = board[current_coord.x][current_coord.y].owner;

	while(current_coord.isValid() && current_coord_move_type == type && num_tiles_in_a_row < WINNING_NUM_OF_TILES && num_tiles_inspected <= MAX_TILES_PER_DIRECTION) {
		num_tiles_in_a_row++;
		current_coord.y++;
		current_coord_move_type = board[current_coord.x][current_coord.y].owner;
		num_tiles_inspected++;
	}
}

bool Gomoku::fiveVertically(Coordinate coord) {
  int num_tiles_in_a_row = 1;  // if this == 5, the game is won!

  checkAboveMove(coord, num_tiles_in_a_row);

  checkBelowMove(coord, num_tiles_in_a_row);

  if(num_tiles_in_a_row == 5)
    return true;
  else
    return false;  
}


void Gomoku::checkAboveMove(Coordinate coord, int& num_tiles_in_a_row) {
  MoveType type = board[coord.x][coord.y].owner;
  Coordinate current_coord = coord;           // coordinate of the grid square under current inspection 
  int num_tiles_inspected = 0;                // highest possible value is 4     

  // step (1): check to the left of most recent move 
  current_coord.x = coord.x - 1;  // TODO: adjust for each function
  MoveType current_coord_move_type = board[current_coord.x][current_coord.y].owner; 

  while(current_coord.isValid() && current_coord_move_type == type && num_tiles_inspected <= MAX_TILES_PER_DIRECTION) {

    // after we've made sure the next coordinate is valid, we can safely index to that space

    num_tiles_in_a_row++;
    current_coord.x--;            // TODO: adjust for each function
    num_tiles_inspected++;
    current_coord_move_type = board[current_coord.x][current_coord.y].owner; 
  }
}


void Gomoku::checkBelowMove(Coordinate coord, int& num_tiles_in_a_row) {
  MoveType type = board[coord.x][coord.y].owner;
  Coordinate current_coord = coord;           // grid squares we are inspecting 
  int num_tiles_inspected = 0;

  // step (2): check to the right of the most recent move
  //     note: need to add an extra condition, num_same_tiles <= 4
  current_coord.x = coord.x + 1; 
  MoveType current_coord_move_type = board[current_coord.x][current_coord.y].owner;

  while(current_coord.isValid() && current_coord_move_type == type && num_tiles_in_a_row < WINNING_NUM_OF_TILES && num_tiles_inspected <= MAX_TILES_PER_DIRECTION) {
    num_tiles_in_a_row++;
    current_coord.x++;
    current_coord_move_type = board[current_coord.x][current_coord.y].owner;
    num_tiles_inspected++;
  }
}

bool Gomoku::fiveDiagonally(Coordinate coord) {
  int num_tiles_in_a_row = 1;  // if this == 5, the game is won!

  checkAboveAndRightOfMove(coord, num_tiles_in_a_row);

  checkBelowAndLeftOfMove(coord, num_tiles_in_a_row);

  if(num_tiles_in_a_row == 5)
    return true;
  else
    return false;  
}

void Gomoku::checkAboveAndRightOfMove(Coordinate coord, int& num_tiles_in_a_row) {
  MoveType type = board[coord.x][coord.y].owner;
  Coordinate current_coord = coord;           // coordinate of the grid square under current inspection 
  int num_tiles_inspected = 0;                // highest possible value is 4     

  // step (1): check to the left of most recent move 
  current_coord.x = coord.x - 1;  // TODO: adjust for each function
  current_coord.y = coord.y + 1;
  MoveType current_coord_move_type = board[current_coord.x][current_coord.y].owner; 

  while(current_coord.isValid() && current_coord_move_type == type && num_tiles_inspected <= MAX_TILES_PER_DIRECTION) {

    // after we've made sure the next coordinate is valid, we can safely index to that space

    num_tiles_in_a_row++;
    current_coord.x--;            // TODO: adjust for each function
    current_coord.y++;
    num_tiles_inspected++;
    current_coord_move_type = board[current_coord.x][current_coord.y].owner; 
  }
}

void Gomoku::checkBelowAndLeftOfMove(Coordinate coord, int& num_tiles_in_a_row) {
  MoveType type = board[coord.x][coord.y].owner;
  Coordinate current_coord = coord;           // grid squares we are inspecting 
  int num_tiles_inspected = 0;

  // step (2): check to the right of the most recent move
  //     note: need to add an extra condition, num_same_tiles <= 4
  current_coord.x = coord.x + 1; 
  current_coord.y = coord.y - 1;
  MoveType current_coord_move_type = board[current_coord.x][current_coord.y].owner;

  while(current_coord.isValid() && current_coord_move_type == type && num_tiles_in_a_row < WINNING_NUM_OF_TILES && num_tiles_inspected <= MAX_TILES_PER_DIRECTION) {
    num_tiles_in_a_row++;
    current_coord.x++;
    current_coord.y--;
    current_coord_move_type = board[current_coord.x][current_coord.y].owner;
    num_tiles_inspected++;
  }
}



bool Gomoku::fiveAntiDiagonally(Coordinate coord) {
  int num_tiles_in_a_row = 1;  // if this == 5, the game is won!

  checkAboveAndLeftOfMove(coord, num_tiles_in_a_row);

  checkBelowAndRightOfMove(coord, num_tiles_in_a_row);

  if(num_tiles_in_a_row == 5)
    return true;
  else
    return false;  
}

void Gomoku::checkAboveAndLeftOfMove(Coordinate coord, int& num_tiles_in_a_row) {
  MoveType type = board[coord.x][coord.y].owner;
  Coordinate current_coord = coord;           // coordinate of the grid square under current inspection 
  int num_tiles_inspected = 0;                // highest possible value is 4     

  // step (1): check to the left of most recent move 
  current_coord.x = coord.x - 1;  // TODO: adjust for each function
  current_coord.y = coord.y - 1;
  MoveType current_coord_move_type = board[current_coord.x][current_coord.y].owner; 

  while(current_coord.isValid() && current_coord_move_type == type && num_tiles_inspected <= MAX_TILES_PER_DIRECTION) {

    // after we've made sure the next coordinate is valid, we can safely index to that space

    num_tiles_in_a_row++;
    current_coord.x--;            // TODO: adjust for each function
    current_coord.y--;
    num_tiles_inspected++;
    current_coord_move_type = board[current_coord.x][current_coord.y].owner; 
  }
}
void Gomoku::checkBelowAndRightOfMove(Coordinate coord, int& num_tiles_in_a_row) {
  MoveType type = board[coord.x][coord.y].owner;
  Coordinate current_coord = coord;           // grid squares we are inspecting 
  int num_tiles_inspected = 0;

  // step (2): check to the right of the most recent move
  //     note: need to add an extra condition, num_same_tiles <= 4
  current_coord.x = coord.x + 1; 
  current_coord.y = coord.y + 1;
  MoveType current_coord_move_type = board[current_coord.x][current_coord.y].owner;

  while(current_coord.isValid() && current_coord_move_type == type && num_tiles_in_a_row < WINNING_NUM_OF_TILES && num_tiles_inspected <= MAX_TILES_PER_DIRECTION) {
    num_tiles_in_a_row++;
    current_coord.x++;
    current_coord.y++;
    current_coord_move_type = board[current_coord.x][current_coord.y].owner;
    num_tiles_inspected++;
  }
}

// used for testing
void Gomoku::setMove(int x, int y, MoveType type) {
  board[x][y].owner = type;
}

void Gomoku::printGameState() {
  cout << "   ";
  for (int i =0; i < GRID_LENGTH; ++i){
    cout <<i << " ";
    if( i < 10)
      cout << " ";
  }
  cout << "\n";
  for(int i = 0; i < GRID_LENGTH; ++i) {
    cout << i << " ";
    if(i < 10)
      cout << " ";
    for(int j = 0; j < GRID_LENGTH; ++j) {
      MoveType currentMove = board[i][j].owner;

      if(currentMove == black) {
        std::cout << "B  ";

      } else if (currentMove == white) {
        std::cout << "W  ";
      } else {
        std::cout << "-  ";
      }
    }
    std::cout << "\n";
  }
}

//void Gomoku::observeBoard(int (*gameState)[GRID_LENGTH][GRID_LENGTH]){
//  
//}

Point2f Gomoku::populateBoard(string filename){
  //TODO: observe previous gamestate, and make sure the delta is only 1 
  //spot prevBoard[GRID_LENGTH][GRID_LENGTH];
  
  vector<compositeCircle> knownCircles = observePieces(filename);
	Point2f coords;

  for(unsigned int i = 0; i < knownCircles.size(); ++i){
    double minDist = -1;
    int minX = 0;
    int minY = 0;
		
    for(int j = 0; j < GRID_LENGTH; ++j){
			for(int t = 0; t < GRID_LENGTH; ++t){
				double curDist = norm( board[j][t].loc - Point2f(knownCircles[i].circle[0],knownCircles[i].circle[1]));
				if(minDist < 0 || curDist < minDist){
					minDist = curDist;
					minX = t;
					minY = j;

				}
      }
		}
		if(board[minX][minY].owner == blank){
			enemyMoves.push_back(Coordinate(minX,minY)); 
			board[minX][minY].owner = knownCircles[i].color;
			coords.x = minX;
			coords.y = minY;
			if(knownCircles[i].color != PLAYER_COLOR){
				cout << "detected computer move in populate board, error " << endl; 
				assert(0);
			}	
			cout << "detected piece placed at " << minY << "," << minX << endl;
		}	
	}
	return coords;
}

bool Gomoku::withinRegion( Coordinate point, spot curSpot){
return true;
}

bool Gomoku::isFree(Coordinate location){
  cout << "checking location " << location.x << "," << location.y << endl;
  return (board[location.x][location.y].owner == blank);  
}

