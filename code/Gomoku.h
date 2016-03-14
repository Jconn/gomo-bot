#ifndef GOMOKU_H
#define GOMOKU_H

const int GRID_LENGTH = 18;

class Gomoku{

  char board[GRID_LENGTH][GRID_LENGTH];
  int num_moves_played;

  public:
    void getMove();
    bool isDraw();
};

#endif
