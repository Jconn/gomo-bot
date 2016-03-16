#ifndef GOMOKU_H
#define GOMOKU_H
<<<<<<< Updated upstream
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <cassert>
using namespace std;
=======
#include <iostream>  // cin, cout
#include <string>    // size, tolower
#include <algorithm> // transform
#include <cassert>   // assert

>>>>>>> Stashed changes
const int GRID_LENGTH = 18;

// 'x' denotes that a player has moved in a grid square
// 'o' denotes the other player has moved in a grid square
// 'blank' denotes an empty grid
enum Move {black, white, blank};

<<<<<<< Updated upstream
enum MoveType {computer=0, human=1, blank=2};

class Loc{
  public:
    int x;
    int y;
    Loc(){
      x=0;
      y=0;
    }
    Loc(int xloc, int yloc){
      x=xloc;
      y=yloc;
    }
};

class Gomoku{
=======
const Move PLAYER_COLOR = black;
const Move AI_COLOR = white;

class Gomoku{
  
  Move board[GRID_LENGTH][GRID_LENGTH];
  int numMovesPlayed;
  bool winnerDetermined;
>>>>>>> Stashed changes

  MoveType board[GRID_LENGTH][GRID_LENGTH];
  int num_moves_played;
  public:
<<<<<<< Updated upstream
  Loc getMove();
  bool setMove(Loc move, MoveType player)
  {
    if(board[move.x][move.y] != blank){
      cout <<"tried to move to occupied state" << endl;
      assert(1);
      return false; 
    }
    board[move.x][move.y] = player; 
    return true;
  }
  bool isDraw();
  void AnalyzeAttacks();
  int get_moves_played() const {return num_moves_played;}
  void set_moves_played(int n) {num_moves_played = n;}

  Gomoku() {
    num_moves_played = 0;

    // initialize all board values to blank
    for(int i = 0; i < GRID_LENGTH; i++) {
      for (int j = 0; j < GRID_LENGTH; j++) {
        board[i][j] = blank;
      }
    }
  }
};
=======
    void getHumanMove();
    bool isDraw();
    
    int getNumMovesPlayed() const {return numMovesPlayed;}
    void incrementNumMovesPlayed() {numMovesPlayed++;}


    bool getWinnerDetermined() const {return winnerDetermined;}
    void setWinnerDetermined() {winnerDetermined = true;}

    Gomoku();

};


Gomoku::Gomoku() {
      // initialize all board values to blank
      for(int i = 0; i < GRID_LENGTH; i++) {
        for (int j = 0; j < GRID_LENGTH; j++) {
          board[i][j] = blank;
        }
      }
      
      numMovesPlayed = 0;
      gameEnded = false;
}

void Gomoku::getHumanMove() {
  
  int horizCoord = 0;
  int vertCoord = 0;

  std::cout << "Where would you like to move? Enter the horizontal coordinate in [1,18].\n";
  std::cin >> horizCoord;

  std::cout << "Where would you like to move? Enter the vertical coordinate in [1,18].\n";
  std::cin >> vertCoord;
 
  

  /*
  std::string moveCoord = "0";
  std::cin >> moveCoord;
  assert(moveCoord.size()  <= 3 && moveCoord.size() >= 2); // must be size 2 or 3
  std::transform(moveCoord.begin(), moveCoord.end(), moveCoord.begin(), ::tolower);

  std::cout << "moveCoord\n";
  */
  
  incrementNumMovesPlayed();

}

bool Gomoku::isDraw() {

  if(numMovesPlayed == GRID_LENGTH * GRID_LENGTH && !winnerDetermined)
    return true;
  else
    return false;
}
>>>>>>> Stashed changes

#endif
