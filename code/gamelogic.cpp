#include "Gomoku.h"



//where the magic happens
//
//can either: 
//  -move randomly
//  -move intelligently
//
//
//
//  AI:
//    search through list of attacking and defending threats
//    if attacking threats result in a winning move, keep going
//    if not, evaluate defensive threats
//      if we need to defend, or else we lose, do so
//      else, attack
//
//
//    attack logic:
//      -paper describes the use of a tree:
//        -set of root nodes, with certain depth, each child dependent on initial move
//        -each threat has a gain square and a cost square
//          -gain square: attacker's move
//          -cost square: defender move in response to threat
//          -rest square: the non-gain squares in a threat
//          -trees in conflict: 
//            -when they have conflicting cost squares
//            -when they have a gain/cost conflict (helping one hurts the other) 
//    
//
//    types of threats:
//      -implication: non-attacker occupied squared in threat are empty
//      -straight four: middle four of 6 are occupied, cannot defend
//      -four: any four of 5 are occupied
//      -three: 3 consecutive  of middle 4 of 6 are occupied
//      -broken3: nonconsecutive 3 of middle 4 of 6 are occupied 

//I hatee everything
Loc Gomoku::getMove()
{


}

void Gomoku::AnalyzeAttacks(){


}
