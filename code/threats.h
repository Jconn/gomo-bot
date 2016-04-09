#ifndef THREATS_H
#define THREATS_H

#include <iostream>      // cin, cout
#include <time.h>        // time
#include <stdlib.h>      // srand, rand 
#include <cassert>       // assert
#include <vector>        // vector
#include <algorithm>     // remove
#include "Coordinate.h"
#include "Constants.h"
#include "MoveType.h"
#include "Gomoku.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "Spot.h"


//stores threats for the AI
using namespace std;
enum ThreatType{three=2, bthree=1,four=3,sfour=4,five=5,none=0 };

/*
   class ThreatTree{
   vector<Threat*> totalThreats; 
   vector<Threat> curThreats;  
   ThreatTree(MoveType player);

   void analyzeThreats(spot board[GRID_LENGTH][GRID_LENGTH],int layers);
   Coordinate recommendMove();

   };
   */

//assume the opponent can do all cost squares at once
//proceed by first making gain move, and then apply all cost moves  
class Threat {
  private:
    void setWinning(){
      winningDepth = -1; 
      if(type == sfour || type == five)
        winningThreat = true;
      else
        winningThreat = false;
    }
  public:
    void printThreat();
    vector <Threat*> totalThreats;
    vector <Threat> children;
    ThreatType type;
    Coordinate  gainSquare;
    Coordinate costSquare[4];
    int numCosts;
    spot board[GRID_LENGTH][GRID_LENGTH]; 
    float selfEvaluation(); 
    MoveType player;
    Threat(ThreatType myType);
    Threat(int x, int y, spot gameState[GRID_LENGTH][GRID_LENGTH]);
    bool winningThreat;
    int winningDepth; 
    bool isNewThreat(Threat &discoveredThreat); 
    void findChildThreats(int depth);
    void setThreatList(vector<Threat*> curThreats); 
    Threat
      (
       ThreatType myType, //the type of threat we are
       Coordinate gain, //this threat's gain square
       Coordinate *costs, //the list of cost squares for this threat
       int costTotal, //the number of costs squares we have
       spot gameState[GRID_LENGTH][GRID_LENGTH], //current state of the game)
      MoveType whoseThreats
        );

    Threat(
        ThreatType myType,
        spot gameState[GRID_LENGTH][GRID_LENGTH],
        MoveType whoseThreat
        );
    //populate the child threats vec

    Threat(Threat * const parent,ThreatType myType);
    void setValue(Coordinate gain, Coordinate *costs, int _numCosts);

};
inline bool operator==(const Threat&  lhs, const Threat& rhs){
  if(lhs.type!=rhs.type)
    return false;
  if(lhs.gainSquare!=rhs.gainSquare)
    return false;
  /* 
     if(lhs.numCosts != rhs.numCosts)
     return false;
     int equal = 0; 
     for(int i =0; i < lhs.numCosts; ++i){
     for(int j = 0; j < lhs.numCosts; ++j){
     if(lhs.costSquare[i] == rhs.costSquare[j])
     ++equal;
     }
     }
     if(equal < lhs.numCosts)
     return false;
     */ 
  return true;
}

inline bool operator!=(const Threat&  lhs, const Threat& rhs){
  return( !(lhs==rhs));
} 

extern void findInitThreats(Coordinate curMove, vector<Threat> &curThreats, spot board[GRID_LENGTH][GRID_LENGTH],MoveType player);
extern Coordinate findBestMove(vector<Threat> &curThreats);
extern bool findBestDefense(vector<Threat> &curThreats, Coordinate &move);


#endif
