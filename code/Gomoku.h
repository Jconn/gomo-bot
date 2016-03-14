#ifndef GOMOKU_H
#define GOMOKU_H
#include <algorithm>
#include <vector>

const int GRID_LENGTH = 18;

// 'x' denotes that a player has moved in a grid square
// 'o' denotes the other player has moved in a grid square
// 'blank' denotes an empty grid

enum Move {x, o, blank};

class Gomoku{
  
  Move board[GRID_LENGTH][GRID_LENGTH];
  int num_moves_played;

  public:
    void getMove();
    bool isDraw();
    
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
