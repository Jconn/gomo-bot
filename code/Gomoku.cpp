#include <iostream>      // cin, cout
#include <time.h>        // time
#include <stdlib.h>      // srand, rand 
#include <cassert>       // assert
#include <vector>        // vector
#include <algorithm>     // remove
#include <unistd.h>
#include <fstream>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "Coordinate.h"
#include "Constants.h"
#include "MoveType.h"
#include "threats.h"
#include "grid.h"
#include "observepieces.h"
#include "UI.cpp"

#include "threatMove.h"


using namespace std;
using namespace cv;

Gomoku::Gomoku(char c) {
  switch(c) {
    case '2':
      gameMode = twoPlayer;
      break;
    case '3':
      gameMode = threePlayer;
      break;
    case '4':
      gameMode = fourPlayer; 
      break;
    default: 
      cout << "Error, please enter one of '2', '3', or '4'.\n";
      assert(0);
  }

  // initialize all board values to blank
  for(int i = 0; i < GRID_LENGTH; i++) {
    for (int j = 0; j < GRID_LENGTH; j++) {
      board[i][j].owner = blank;
    }
  }

  numMovesPlayed = 0;
  gameEnded = false;

  //if(!populateGridEdges("../images/pierpont/connector/001.jpg",board))
  //	assert(0);
  // 
  //	if(!populateGridEdges("../images/JJgame/000.jpg",board))
  // 	assert(0);

  // begin code to take empty board image
  system("raspistill -t 1000 -n -o empty.jpg");
  system("raspistill -t 1000 -n -o move.jpg");	
  if(!populateGridEdges("empty.jpg", board))
    assert(0);

  initializePlaybook();
}

Coordinate Gomoku::getHumanMove(vector<Coordinate>& us, int player_num) {
  Coordinate coord(0,0);

  // really hacky, ugly way
  string playerColor_str[] = {
    "White",
    "Green",
    "Red"
  };

  MoveType playerColor_mt[] = {
    white,
    green,
    red
  };

  bool validMove = false;

  while(!validMove){
    std::cout << playerColor_str[player_num] << "'s move. \nWhere would you like to move?\n";
    
    std::cout << "Enter the horizontal coordinate in [0,18].\n";
    std::cin >> coord.y;

    std::cout << "Enter the vertical coordinate in [0,18].\n";
    std::cin >> coord.x;

    std::cout << "size: " << us.size() << '\n';

    us.erase(std::remove(us.begin(), us.end(), coord), us.end());
    std::cout << "size: " << us.size() << '\n';

    MoveType proposedMoveType = board[coord.x][coord.y].owner;

    if(proposedMoveType == blank){
      board[coord.x][coord.y].owner = playerColor_mt[player_num];
      validMove = true;
    } else {
      std::cout << "error, this space already occupied sux 4 u\n";
    }
  }
  enemyMoves.push_back(coord); 
  incrementNumMovesPlayed();

  return coord;
}


Coordinate Gomoku::getAIMove() {
  cout << " grabbing ai move " << endl;	
  Coordinate curMove;

  //check what mode the AI has been set to in order to determine
  //our AI logic for finding a move
  if(myMode == randomAI){
    assert(1); 
    //myMoves.push_back(getRandomAIMove(NULL));
    curMove = myMoves[myMoves.size()-1];
  } else {
    cout << "myMoves size is " << myMoves.size() << endl;
    if(myMoves.size() < 2){
      cout << "going into opening playbook" << endl; 
      for(unsigned int i = 0; i < openingPlaybook.size(); ++i) {
        if(isFree(openingPlaybook[i])) {
          cout << "moving to " << openingPlaybook[i].x <<"," << openingPlaybook[i].y << endl; 
          myMoves.push_back(openingPlaybook[i]);
          setAIMove(myMoves[myMoves.size()-1]);
          return myMoves[myMoves.size()-1];
        }
      }
    }

    potentialMove defThreat;
    potentialMove offThreat;
    if(myMode == defensive){
      defThreat= getDefense();
      myMoves.push_back(defThreat.move);
      curMove = myMoves[myMoves.size()-1];
    }
    if(myMode == intelligent){
      defThreat = getDefense();
      offThreat = getAttack();

      cout << " best defensive threat found: ";
      cout << " location is: ("<<defThreat.move.x << "," << defThreat.move.y << ")" << endl;
      cout << "winnning is: " << defThreat.isWinning << endl;
      if(defThreat.isWinning)
        cout << "winnning depth is: " << defThreat.depth << endl;

      cout << " best offensive threat found: ";
      cout << " location is: ("<<offThreat.move.x << "," << offThreat.move.y << ")" << endl;
      cout << "winnning is: " << offThreat.isWinning << endl;
      if(offThreat.isWinning)
        cout << "winnning depth is: " << offThreat.depth << endl;


      if(defThreat.isWinning && offThreat.isWinning){
        cout << " found a winning and losing threat" << endl;
        if(offThreat.depth <= defThreat.depth)
          myMoves.push_back(offThreat.move);
        else
          myMoves.push_back(defThreat.move);
      }
      else if(offThreat.isWinning)
        myMoves.push_back(offThreat.move);
      else if(defThreat.isWinning && defThreat.depth<=1)
        myMoves.push_back(defThreat.move);
      else
        myMoves.push_back(offThreat.move);
      assert(myMoves.size());
      curMove = myMoves[myMoves.size()-1]; 
    }
  }
  curMove = myMoves[myMoves.size()-1];
  setAIMove(curMove);

  return curMove;}


  void Gomoku::initializePlaybook(){
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

    //Call UI
    //Mat src = imread("move.jpg", 0); 
    //UI(src, coord, board);
  }
  incrementNumMovesPlayed();
}

potentialMove Gomoku::getDefense(){
  //we have the board, loop through our spaces, and find threats that we have,
  //but at the start of our play, just follow the playbook
  //	cout << "enemyMoves size is " << enemyMoves.size() << endl;

  //check to see if we've made less than 2 moves, and if so, 
  //make a move based on the playbook

  vector <Threat> threats;


  //loop through the enemyMoves vector to determine if any of the 
  //enemy's currently placed pieces can form a threat
  //
  //TODO: extend this to multi-player
	cout << "finding initial threats.. " << endl;
	for(unsigned int i = 0; i < enemyMoves.size(); ++i){ 
		findInitThreats(enemyMoves[i],threats,board,PLAYER_COLOR);
	}
 
	for(int i = 0; i < static_cast<int>(threats.size()); ++i){
		if(threats[i].winningThreat){
			return threats[i].returnMove();
		}
	}
  
  for(unsigned int i = 0; i < threats.size(); ++i){
    threats[i].printThreat(); 
    threats[i].findChildThreats(5); 
  }

  int bestIndex = -1;
  int bestDepth = 0;
  for(int i = 0; i < static_cast<int>(threats.size()); ++i){
    if(threats[i].winningThreat && threats[i].winningDepth > bestDepth){
      bestIndex = i;
      bestDepth = threats[i].winningDepth;
    }
  }
  if(bestIndex >=0)
    return (threats[bestIndex]).returnMove();

  //after we've found the list of threats that the enemy has,
  //we use a heuristic to determine the most damaging threat
  //by calling the function findBestDefense()
  //
  //If we can't find a good defensive move to be made, 
  //then we call getAttack() to get the best attack move 
  if( threats.size() > 0){ 
    return (findBestMove(threats));
  }

  //if we couldn't spot any threats by enemy, just make an attack move
  return getAttack();
}


potentialMove Gomoku::getAttack(){
  //we have the board, loop through our spaces, and find threats that we have,
  //but at the start of our play, just follow the playbook


  //find initial threats that the computer can make
  vector <Threat> threats;  
  cout << "finding initial threats.. " << endl;
  for(unsigned int i = 0; i < myMoves.size(); ++i){ 
    findInitThreats(myMoves[i],threats,board,AI_COLOR);
  }

  //if any threat sequence has been found to win for the computer,
  //then return that immediately
  for(int i = 0; i < static_cast<int>(threats.size()); ++i){
    if(threats[i].winningThreat){
      return threats[i].returnMove();
    }
  }


  //now have the initial threats;

  //populates the vector that points to all current threats,
  //needed for finding new threats
  vector <Threat*> threatPtrs;
  for(unsigned int i = 0; i < threats.size(); ++i){
    threatPtrs.push_back(&(threats[i]));
  }


  cout <<"detected " << threats.size() << " initial threats " << endl;


  for(unsigned int i = 0; i < threats.size(); ++i){
    threats[i].printThreat(); 
    threats[i].findChildThreats(5); 
  }
  cout << " finished child threats " << endl;
  int bestDepth = -1;
  int bestIndex = -1;

  //check if populating threat-tree found us a winning 
  //sequence, and return the winning sequence immediately
  //if found
  for(int i = 0; i < static_cast<int>(threats.size()); ++i){
    if(threats[i].winningThreat && threats[i].winningDepth > bestDepth){
      bestIndex = i;
      bestDepth = threats[i].winningDepth;
    }
  }
  if(bestIndex >=0)
    return (threats[bestIndex]).returnMove();

  if( threats.size() > 0) 
    return findBestMove(threats);


  //we did not find a winning sequence, we did not even find a threat
  //so we just add every free space in the board, and 
  //then extrapolate child threats from there
  //time to get dirty
  //TODO: stop being dirty
  for(int i =0; i <GRID_LENGTH; ++i){
    for(int j = 0; j < GRID_LENGTH; ++j){
      if(board[i][j].owner==blank){
        Threat newThreat(i,j,board);
        threats.push_back(newThreat);
      }

    }
  }

  //run the search tree five layers deep
  cout << "attempting to find best move among s--ers" << endl;
  cout << "threat size is " <<threats.size() << endl;
  for(unsigned int i = 0; i < threats.size(); ++i){
    threats[i].findChildThreats(5); 
  }


  //if we find a threat, then call findBestMove(),
  //which is a heuristic to determine the best threat
  //we can make
  assert(threats.size());
  return findBestMove(threats);
}

vector <Threat> getThreats(){
  //loop through our moves, call each move a "threat"
  //and then after each move is called a "threat"
  //determine their children, these are our actual initial threats
  //parse all the children together, and return a vector of these coordinates
  //TODO: erase this fn probably 
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

  //this function just prints out the game state 
  //with ascii characters
  //all of the conditionals that are used (if i < 10)
  //just accounts for the fact that some numbers will take 
  //two chars to print and others take one char to print
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
      } else if(currentMove == green) {
        std::cout << "G  ";
      } else if(currentMove == red) {
        std::cout << "R ";
      } else {
        std::cout << "-  ";
      }
    }
    std::cout << "\n";
  }
}

// returns bool indicating whether or not human's piece was found
bool Gomoku::populateBoard(string filename, Coordinate &enemyMove){

  //this function takes the path to a file name and returns two things:
  //-boolean that indicates whether or not the number of pieces found
  //was the number of pieces expected
  //-updates enemyMove to indicate where the other player moved,
  //this is used in main so we can check to see if a winning move was made
  //TODO: update the enemyMove update to be able to accomodate multi-player
  //	vector<compositeCircle> knownCircles = observePieces(filename,numMovesPlayed+1 );

  //knownCircles stores all of the observed pieces, and the players
  //that each piece belongs to

  //second arg indicates how many pieces we expect to see,
  //use -1 as the second arg for debug mode
  //  this tells the function to just find as many pieces as 
  //   it can, and then it calls imshow to show the developer
  //   which pieces were detected
  // vector<compositeCircle> knownCircles = observePieces(filename,-1 );
  vector<compositeCircle> knownCircles = observePieces(filename,numMovesPlayed+1 );

  Point2f coords;
  MoveType foundColor;
  bool foundPiece = false;
  //loop through each observed piece in knownCircles, and
  //check to see whether or not the piece is new
  for(unsigned int i = 0; i < knownCircles.size(); ++i){

    double minDist = -1;
    int minX = 0;
    int minY = 0;

    for(int j = 0; j < GRID_LENGTH; ++j){
      for(int t = 0; t < GRID_LENGTH; ++t){
        //determine the distance from the center of the observed 
        //piece to every space in the board
        //store the min distance
        double curDist = norm( board[j][t].loc - Point2f(knownCircles[i].circle[0],knownCircles[i].circle[1]));
        if(minDist < 0 || curDist < minDist){
          minDist = curDist;
          minX = t;
          minY = j;
        }
      }
    }

    // if the space that this detected piece is closest to 
    // is a blank space, then we know it is a new piece
    if(board[minX][minY].owner == blank){

      //check to see if the new piece that was found was a computer piece. If it is,
      //something went wrong
      if(knownCircles[i].color != PLAYER_COLOR){
        cout << "computer was moved to the wrong spot, error " << endl; 
        cout << " computer spot that we found is at location: " << minX << "," << minY << endl;	
        return false;
      }	

      //check to see if we have already found a piece,
      //if we did then we have spotted >1 new pieces placed and
      //return false to indicate something went wrong
      if(foundPiece ==true){
        cout << "found more than one piece in populateBoard " << endl;
        cout << "please make sure only one piece was played " << endl;
        cout << " second spot that we found is at location: " << minX << "," << minY << endl;			
        return false;  
      }
      foundPiece = true;	
      enemyMove.x = minX;
      enemyMove.y = minY;	
      coords.x = minX;
      coords.y = minY;
      foundColor = knownCircles[i].color;


      cout << "detected piece placed at " << minY << "," << minX << endl;
    }	
  }

  //if we did find a new piece, then update the game state 
  //with this new piece
  if(foundPiece==true){
    incrementNumMovesPlayed();
    enemyMoves.push_back(Coordinate(coords.x,coords.y)); 
    int x  = coords.x; 
    int y = coords.y;	
    board[x][y].owner = foundColor;
  }
  return foundPiece;
}

bool Gomoku::withinRegion( Coordinate point, spot curSpot){
  return true;
}

bool Gomoku::isFree(Coordinate location){
  cout << "checking location " << location.x << "," << location.y << endl;
  return (board[location.x][location.y].owner == blank);  
}
/*
   bool Gomoku::isBlank(Coordinate location){
   cout << "checking location " << location.x << "," << location.y << endl;
   return (board[location.x][location.y].owner == blank);  
   }
   */
