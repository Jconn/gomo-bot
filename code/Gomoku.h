#ifndef GOMOKU_H
#define GOMOKU_H

#include <iostream>      // cin, cout
#include <time.h>        // time
#include <stdlib.h>      // srand, rand 
#include <cassert>       // assert
#include <vector>        // vector
#include "Coordinate.h"
#include "Constants.h"
#include "MoveType.h"

using namespace std;
struct spot{
  MoveType owner;
  Coordinate topLeft;
  Coordinate topRight;
  Coordinate botLeft;
  Coordinate botRight;
  spot(){
    owner = blank;
  }
};
class Gomoku{

  int numMovesPlayed;
  bool winnerDetermined;
  spot board[GRID_LENGTH][GRID_LENGTH]; // (0,0) corresponds to the top left of the board
  
   
  // 2 ways to be true:
  // (1) before all grid squares are filled, one player gets 5 in a row
  // (2) all grid squares filled without a player getting 5 in a row
  bool gameEnded; 

  public:
    Gomoku();

    Coordinate getHumanMove();
    //void observeBoard(int (*gameState)[GRID_LENGTH][GRID_LENGTH]);    
    int getNumMovesPlayed() const {return numMovesPlayed;}
    void incrementNumMovesPlayed() {numMovesPlayed++;}

    bool getWinnerDetermined() const {return winnerDetermined;}
    void setWinnerDetermined() {winnerDetermined = true;}

    bool getGameEnded() const {return gameEnded;}
    void setGameEnded() {gameEnded = true;}

    Coordinate getRandomAIMove();

    bool isDraw();

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


Gomoku::Gomoku() {
  // initialize all board values to blank
  for(int i = 0; i < GRID_LENGTH; i++) {
    for (int j = 0; j < GRID_LENGTH; j++) {
      board[i][j].owner = blank;
    }
  }
  numMovesPlayed = 0;
  gameEnded = false;
}

Coordinate Gomoku::getHumanMove() {
  Coordinate coord(0,0);

  std::cout << "Where would you like to move? Enter the horizontal coordinate in [1,18].\n";
  std::cin >> coord.x;

  std::cout << "Where would you like to move? Enter the vertical coordinate in [1,18].\n";
  std::cin >> coord.y;
 
  MoveType proposedMoveType = board[coord.x][coord.y].owner;

  if(proposedMoveType == blank)
    board[coord.x][coord.y].owner = PLAYER_COLOR;
  else {
    std::cout << "error, this space already occupied sux 4 u\n";
    assert(0);
    return coord;
  }
  incrementNumMovesPlayed();

  return coord;
}

// TODO: this method is horrid, fix it
// have a vector of all remaining places, generate an index
Coordinate Gomoku::getRandomAIMove() {
  Coordinate coord(0,0);

  srand(time(NULL));
  
  while(1) {
    coord.x = rand() % 18 + 1;

    coord.y = rand() % 18 + 1;
    
    if(board[coord.x][coord.y].owner == blank) {
      std::cout << "random AI moves to " << coord.x << ", " << coord.y << "\n";
      board[coord.x][coord.y].owner = AI_COLOR;
      break;
    }
  }
  incrementNumMovesPlayed();

  return coord;
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
  for(int i = 0; i < GRID_LENGTH; ++i) {
    for(int j = 0; j < GRID_LENGTH; ++j) {
      MoveType currentMove = board[i][j].owner;
      
      if(currentMove == black) {
        std::cout << "B ";

      } else if (currentMove == white) {
        std::cout << "W ";
      } else {
        std::cout << "- ";
      }
    }
    std::cout << "\n";
  }
}

//void Gomoku::observeBoard(int (*gameState)[GRID_LENGTH][GRID_LENGTH]){


//}


#endif // GOMOKU_H
