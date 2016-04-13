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
#include "threatMove.h"

//stores threats for the AI
using namespace std;
enum ThreatType{three=7, bthree=3,four=8,sfour=14,five=15,none=0 };

//assume the opponent can do all cost squares at once
//proceed by first making gain move, and then apply all cost moves  
class Threat {
  private:
    void setWinning(){
      winningDepth = -1; 
      if(type == five){
        winningThreat = true;
        winningDepth = layersDeep;
      }
      else if(type== sfour){
        winningThreat = true;
        
        //if we don't set winning depth to +1 in the sfour case
        //then the ai evaluates itself as being able to win
        //too early in some cases, and doesn't block player move
        //
        winningDepth = layersDeep + 1;
      }
      else
        winningThreat = false;
    }
  public:
    void printThreat();
    
    //this is used to make sure we don't discover a threat more than once
    vector <Threat*> totalThreats;
    vector <Threat> children;
    ThreatType type;
    Coordinate  gainSquare;
    Coordinate costSquare[4];
    int numCosts;
    spot board[GRID_LENGTH][GRID_LENGTH]; 
    float selfEvaluation(); 
    MoveType player;
    potentialMove returnMove(); 
    Threat(ThreatType myType);
    Threat(int x, int y, spot gameState[GRID_LENGTH][GRID_LENGTH]);
    bool winningThreat;
    int winningDepth; 
    bool isNewThreat(Threat &discoveredThreat); 
    void findChildThreats(int depth);
    void setThreatList(vector<Threat*> curThreats); 
    int layersDeep=0;
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
    Threat();
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
extern potentialMove findBestMove(vector<Threat> &curThreats);


#endif
