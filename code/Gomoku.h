#ifndef GOMOKU_H
#define GOMOKU_H
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <cassert>
using namespace std;
const int GRID_LENGTH = 18;

// 'x' denotes that a player has moved in a grid square
// 'o' denotes the other player has moved in a grid square
// 'blank' denotes an empty grid

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

  MoveType board[GRID_LENGTH][GRID_LENGTH];
  int num_moves_played;
  public:
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

#endif
