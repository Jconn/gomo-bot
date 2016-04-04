#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "MoveType.h"

const int GRID_LENGTH = 18;
const int MAX_TILES_PER_DIRECTION = 4; // upper limit of the # of tiles to inspect when determining if a move is a winning move
const int WINNING_NUM_OF_TILES = 5;

const MoveType PLAYER_COLOR = white;
const MoveType AI_COLOR = black;

#endif
