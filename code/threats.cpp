#include "threats.h" 



//pass in your gain square to this fn 
inline void printLoc(Coordinate &curcoord){
cout << " " << curcoord.x << "," << curcoord.y << " ";
}
inline bool isUnique(Threat posThreat, vector<Threat> &curThreats){
  bool unique = true; 
  for(unsigned int i = 0; i < curThreats.size(); ++i){
    if(posThreat == curThreats[i]){
      unique = false; 
    }
  }
  return unique;
}
float Threat::selfEvaluation(){
  //look at our threat type, and how many children we have, and the types of our children
  int numChildren = children.size(); 
  float total = 0;
  for(int i = 0; i < numChildren; ++i){
    total+=(children[i].type/2.0);
  }
  total+=numChildren + 2.0*type;
  return  total;
}
Coordinate findBestMove(vector<Threat> &curThreats){
  float maxThreat =-1;
  int maxIndex = 0;
  for(int i =0; i < (int)curThreats.size(); ++i){
    if(curThreats[i].selfEvaluation() > maxThreat){
      maxThreat = curThreats[i].selfEvaluation();
      maxIndex = i;
    }
  }
  return curThreats[maxIndex].gainSquare;
} 

bool findBestDefense(vector<Threat> &curThreats, Coordinate &move){
  float maxThreat =-1;
  int maxIndex = 0;
  for(int i =0; i < (int)curThreats.size(); ++i){
    if(curThreats[i].selfEvaluation() > maxThreat){
      maxThreat = curThreats[i].selfEvaluation();
      maxIndex = i;
    }
  }
  assert(curThreats[maxIndex].numCosts > 0);
  if(maxThreat <= 2)
    return false;
  move.x = curThreats[maxIndex].gainSquare.x;
  move.y = curThreats[maxIndex].gainSquare.y;
  return true; 
}

Threat::Threat(int x, int y, spot gameState[GRID_LENGTH][GRID_LENGTH])
{
  //the constructor you call when the ai is in a bad spot
  type = none; 
  for(int i = 0; i < GRID_LENGTH; ++i){
    for(int j= 0; j < GRID_LENGTH; ++j){
      board[i][j] = gameState[i][j];
    }
  }
  player = AI_COLOR;  
  Coordinate gain(x,y);
  gainSquare =gain; 
  numCosts = 0;
  setWinning();
}

  Threat::Threat
(
 ThreatType myType, //the type of threat we are
 Coordinate gain, //this threat's gain square
 Coordinate *costs, //the list of cost squares for this threat
 int costTotal, //the number of costs squares we have
 spot gameState[GRID_LENGTH][GRID_LENGTH], //current state of the game)
  MoveType whoseThreats
  )
  :
    type(myType),
    gainSquare(gain),
    numCosts(costTotal),
    player(whoseThreats)
{
  for(int i = 0; i < GRID_LENGTH; ++i){
    for(int j= 0; j < GRID_LENGTH; ++j){
      board[i][j] = gameState[i][j];
    }
  }
  for (int i= 0; i < numCosts; ++i){
    costSquare[i] = costs[i];
  }
  setWinning();
}
Threat::Threat(
    ThreatType myType,
    spot gameState[GRID_LENGTH][GRID_LENGTH],
    MoveType whoseThreat
    )
  :
    type(myType),
    player(whoseThreat)
{
  for(int i = 0; i < GRID_LENGTH; ++i){
    for(int j= 0; j < GRID_LENGTH; ++j){
      board[i][j] = gameState[i][j];
    }
  }
  setWinning();
}
//populate the child threats vec

Threat::Threat(Threat * const parent,ThreatType myType )
  :
    type(myType),
    player(parent->player)
{
}
void Threat::setValue(Coordinate gain, Coordinate *costs, int _numCosts){
  gainSquare = gain; 
  numCosts = _numCosts;
  for (int i= 0; i < numCosts; ++i){
    costSquare[i] = costs[i];
  }
  board[gain.x][gain.y].owner = player; 
}
void Threat::setThreatList(vector<Threat*> curThreats){
  totalThreats = curThreats;
}

void Threat::printThreat(){
  string threatStr;
  if(type ==three)
    threatStr = "three";
  if(type ==bthree)
    threatStr = "broken three";
  if(type==four)
    threatStr = "four";
  if(type==sfour)
    threatStr = "straight four";
  if(type==five)
    threatStr = "five";
  cout << "threat:\n";
  cout << "threat of type " <<threatStr <<"\n";
  cout << "gainSquare is: " <<gainSquare.x << "," <<gainSquare.y << "\n";
  cout << "num Costs are: " << numCosts << "\n";
  for(int i = 0; i < numCosts; ++i){
  cout << costSquare[i].x << "," << costSquare[i].y <<"\n";
  }
  cout << endl;

}

void Threat::findChildThreats(int depth){
  //loop through the previous move, determine if there are any threats, make sure they are not 
  //threats that we already have,
  //then update it as a new threat
  //we should be okay just storing the type of threat, and the gain/cost squares
  //start with 3 spaces away from our guy, and keep searching
 // printThreat(); 
  board[gainSquare.x][gainSquare.y].owner = player;
  MoveType opp;
  if(player==black)
    opp = white;
  else
    opp = black; 
  cout << "finding children with " << numCosts << " costs " << endl;
  for(int i =0; i < numCosts; ++i){
      board[costSquare[i].x][costSquare[i].y].owner = opp;
  }
  Coordinate curMove = gainSquare; 
  cout << " about to evaluate child threats at depth "<< depth << endl; 
  if(depth==0 || winningThreat){
    cout << " exiting early " << endl; 
    if(winningThreat){      
      cout << "found a winning threat " << endl;
      winningDepth = depth; 
    }
    if(!depth)
      cout << "reached end of threat search " << endl;
    return;
  }
  Direction allDirs[] = {diag,lr,ud,antidiag};

  for(int i = 0; i < 4; ++i){
    Direction curDir = allDirs[i]; 
    Coordinate searchLoc = curMove;
    searchLoc.decrement(curDir, 3);
       bool dirDone = false;
    while(!dirDone){
     
      vector <Coordinate> freeSpaces;
      vector <Coordinate> freeBoundaries;
      vector <Coordinate> threatSpaces; 
      Coordinate curLoc = searchLoc;
      Coordinate endLoc = searchLoc; 
      endLoc.increment(curDir,3);
      int threatPieces= 0; 
      bool broken = false;
      bool done = false;
      while(!done){
        if(board[curLoc.x][curLoc.y].owner==player){
          threatSpaces.push_back(curLoc);
          threatPieces++;
        }
        else if(board[curLoc.x][curLoc.y].owner!=blank){
          //there is a piece in our core 4 that is neither blank
          //nor belonging to us, we are done with this threat now
          broken = true; 
          break; 
        }
        else{
          //space is empty

          freeSpaces.push_back(curLoc);
        }
        if( curLoc==endLoc){
          done = true;
          break;
        }
        if(!curLoc.increment(curDir,1)){
          broken = true;
          break;
        }
      }

      //done with the local search of our core 4 for this iteration,
      //now check if we found anything interesting
      int emptyBoundaries = 0; 
      if(threatPieces >=2 && !broken){
        //we found something promising in core 4, check others 
        Coordinate boundaryLoc; 
        switch(threatPieces){
          case 4:
            //we have 4 unbroken pieces, see if we have space around our piece
            //to make a 4 or straight four
            boundaryLoc = searchLoc;
            if(boundaryLoc.decrement(curDir,1) ){
              //check to see if our boundary is empty  
              if(board[boundaryLoc.x][boundaryLoc.y].owner == blank){
                freeBoundaries.push_back(boundaryLoc); 
                emptyBoundaries++;
              }

            }
            boundaryLoc = endLoc;
            if(boundaryLoc.increment(curDir,1) ){
              //check to see if our boundary is empty  
              if(board[boundaryLoc.x][boundaryLoc.y].owner == blank){
                freeBoundaries.push_back(boundaryLoc);
                emptyBoundaries++;
              }
            }
            if(emptyBoundaries>=1){
              //create a five threat
              //threat piece

              Threat fiveThreat(this,five);
              //set this threat
              fiveThreat.setValue(freeBoundaries[0], NULL, 0); 
              children.push_back(fiveThreat); 
            }
            break;
          case 3:
            //we have 3 out of the middle four available, now we need to see what threats we can make 
            //with this information
            //between searchLoc and endLoc
            boundaryLoc = searchLoc;
            if(boundaryLoc.decrement(curDir,1) ){
              //check to see if our boundary is empty  
              if(board[boundaryLoc.x][boundaryLoc.y].owner == blank){
                emptyBoundaries++;
                freeBoundaries.push_back(boundaryLoc);
              }
            }
            boundaryLoc = endLoc;
            if(boundaryLoc.increment(curDir,1) ){
              //check to see if our boundary is empty  
              if(board[boundaryLoc.x][boundaryLoc.y].owner == blank){
                emptyBoundaries++;
                freeBoundaries.push_back(boundaryLoc);
              }
            }
            if(emptyBoundaries==2){
              //create straight four threat
              Threat sfourThreat(this,sfour);
							//set this threat
							//assert(freeSpaces.size()==1); 
							if((freeSpaces.size()!=1))
								break; 

							sfourThreat.setValue(freeSpaces[0], NULL, 0); 
							if(isNewThreat(sfourThreat)) 
								children.push_back(sfourThreat);  
            }
            if(emptyBoundaries==1){
              //create a four threat
              Threat fourThreat1(this,four);
              Threat fourThreat2(this,four); 
              //set this threat
              if((freeSpaces.size()!=1))
								break;
              fourThreat1.setValue(freeSpaces[0], &(freeBoundaries[0]), 1);
              fourThreat2.setValue(freeBoundaries[0], &(freeSpaces[0]), 1);
              if(isNewThreat(fourThreat1)) 
                children.push_back(fourThreat1);
              if(isNewThreat(fourThreat2)) 
                children.push_back(fourThreat2);             
            }
            break;
          case 5: 
            //NOT POSSIBLE!! 
            break;
          case 2:
            //create some three threats
            boundaryLoc = searchLoc;
            if(boundaryLoc.decrement(curDir,1) ){
              //check to see if our boundary is empty  
              if(board[boundaryLoc.x][boundaryLoc.y].owner == blank){
                freeBoundaries.push_back(boundaryLoc); 
                emptyBoundaries++;
              }
              /*
                 if(boundaryLoc.decrement(curDir,1) ){
            //check to see if our boundary is empty  
            if(board[boundaryLoc.x][boundaryLoc.y] == blank){
            freeBoundaries.push_back(boundaryLoc); 
            emptyBoundaries++;
            }
            }
            */

            }
            boundaryLoc = endLoc;
            if(boundaryLoc.increment(curDir,1) ){
              //check to see if our boundary is empty  
              if(board[boundaryLoc.x][boundaryLoc.y].owner == blank){
                freeBoundaries.push_back(boundaryLoc);
                emptyBoundaries++;
              }
              /*
                 if(boundaryLoc.increment(curDir,1) ){
            //check to see if our boundary is empty  
            if(board[boundaryLoc.x][boundaryLoc.y] == blank){
            freeBoundaries.push_back(boundaryLoc); 
            emptyBoundaries++;
            }
            }
            */
            }
            if(emptyBoundaries==2){
              //we can make some threes, 
              // we just need to figure out 
              // what kind of three threat we are making...
              // borken three or three?
              //can create two threats, always. A broken three, or a straight three 
							bool adjacent;
							if(freeSpaces.size()!=2)
								break; 
							threatSpaces.push_back(freeSpaces[0]); 
              for(unsigned int i = 0; i< threatSpaces.size(); ++i){
                adjacent = false; 
                for(unsigned int j = 0; j < threatSpaces.size(); ++j){
                  if(j!=i){
                    if(threatSpaces[i].movesFrom(curDir, threatSpaces[j]) ==1){
                      adjacent = true;
                    }
                  }
                  if(adjacent ==false)
                    break;
                }
              }
              if(adjacent ==false){
                //we have our broken three if adjacent is false
                //and since each three situation can create two threats, we 
                Threat brokenThree(this, bthree); 
                Coordinate threats[3] = {freeSpaces[1],freeBoundaries[0],freeBoundaries[1]}; 
                brokenThree.setValue(freeSpaces[0],threats, 3); 
                if(isNewThreat(brokenThree)) 
                  children.push_back(brokenThree);
              }
              else{
                ThreatType threeThreat = three;
                Threat three(this, threeThreat); 
                Coordinate threats[3] = {freeSpaces[1],freeBoundaries[0],freeBoundaries[1]}; 
                three.setValue(freeSpaces[0],threats, 3); 
                if(isNewThreat(three))
                  children.push_back(three);
              }


              threatSpaces.pop_back();
              //found what one of the threats will be, see what this threat will be
              threatSpaces.push_back(freeSpaces[1]); 
              for(unsigned int i = 0; i< threatSpaces.size(); ++i){
                adjacent = false; 
                for(unsigned int j = 0; j < threatSpaces.size(); ++j){
                  if(j!=i){
                    if(threatSpaces[i].movesFrom(curDir, threatSpaces[j]) ==1){
                      adjacent = true;
                    }
                  }
                  if(adjacent ==false)
                    break;
                }
              }
              if(adjacent ==false){
                //we have our broken three if adjacent is false
                //and since each three situation can create two threats, we 
                Threat brokenThree(this, bthree); 
                Coordinate threats[3] = {freeSpaces[0],freeBoundaries[0],freeBoundaries[1]}; 
                brokenThree.setValue(freeSpaces[1],threats, 3); 
                if(isNewThreat(brokenThree)) 
                  children.push_back(brokenThree);
              }
              else{
                ThreatType threeThreat = three;
                Threat three(this,threeThreat); 
                Coordinate threats[3] = {freeSpaces[0],freeBoundaries[0],freeBoundaries[1]}; 
                three.setValue(freeSpaces[1],threats, 3); 
                if(isNewThreat(three))
                  children.push_back(three);
              }

            }
            break;
        }
      }

      if(searchLoc==curMove)
        dirDone = true;
      searchLoc.increment(curDir,1);

    }

  }
  //now iterate through our children, and update their found threat list
  for(unsigned i = 0; i < children.size(); ++i){
    children[i].totalThreats = totalThreats;
  }
  cout << " total children of this threat is " << children.size() << endl;
  cout << " current depth is " << depth << endl;
  for(unsigned i = 0; i < children.size(); ++i){ 
    children[i].findChildThreats(depth-1);  
  }
  for(unsigned i = 0; i < children.size(); ++i){
    if(children[i].winningThreat == true)
      winningThreat = true;
  }
}



//pass in the player in the MoveType argument
//TODO: JT make sure you call this for each player when we look for threats from other players
void findInitThreats(Coordinate curMove, vector<Threat> &curThreats, spot board[GRID_LENGTH][GRID_LENGTH],MoveType player){
  //loop through the previous move, determine if there are any threats, make sure they are not 
  //threats that we already have,
  //then update it as a new threat
  //we should be okay just storing the type of threat, and the gain/cost squares
  //start with 3 spaces away from our guy, and keep searching
  MoveType myColor = player;
  Direction allDirs[] = {diag,lr,ud,antidiag};
  //start at our last moved location, and see if any new threats have been created because of us 
  for(int i = 0; i < 4; ++i){
    Direction curDir = allDirs[i]; 
    Coordinate searchLoc = curMove;
    searchLoc.decrement(curDir, 3);
   
    bool dirDone = false;
    while(!dirDone){
      
      vector <Coordinate> freeSpaces;
      vector <Coordinate> freeBoundaries;
      vector <Coordinate> threatSpaces; 
      Coordinate curLoc = searchLoc;
      Coordinate endLoc = searchLoc; 
      endLoc.increment(curDir,3);
      int threatPieces= 0; 
      bool broken = false;
      bool done = false;
      while(!done){
        if(board[curLoc.x][curLoc.y].owner==player){
          threatSpaces.push_back(curLoc);
          threatPieces++;
        }
        else if(board[curLoc.x][curLoc.y].owner!=blank){
          //there is a piece in our core 4 that is neither blank
          //nor belonging to us, we are done with this threat now
          broken = true; 
          break; 
        }
        else{
          //space is empty

          freeSpaces.push_back(curLoc);
        }
        if( curLoc==endLoc){
          done = true;
          break;
        }
        if(!curLoc.increment(curDir,1)){
          broken = true;
          break;
        }
      }
      //done with the local search of our core 4 for this iteration,
      //now check if we found anything interesting
      int emptyBoundaries = 0; 
      if(threatPieces >=2 && !broken){
        //we found something promising in core 4, check others 
        Coordinate boundaryLoc; 
        switch(threatPieces){
          case 4:
            //we have 4 unbroken pieces, see if we have space around our piece
            //to make a 4 or straight four
            boundaryLoc = searchLoc;
            if(boundaryLoc.decrement(curDir,1) ){
              //check to see if our boundary is empty  
              if(board[boundaryLoc.x][boundaryLoc.y].owner == blank){
                freeBoundaries.push_back(boundaryLoc); 
                emptyBoundaries++;
              }

            }
            boundaryLoc = endLoc;
            if(boundaryLoc.increment(curDir,1) ){
              //check to see if our boundary is empty  
              if(board[boundaryLoc.x][boundaryLoc.y].owner == blank){
                freeBoundaries.push_back(boundaryLoc);
                emptyBoundaries++;
              }
            }
            if(emptyBoundaries>=1){
              //create a five threat
              //threat piece
              Threat fiveThreat(five,board,myColor );
              //set this threat
              fiveThreat.setValue(freeBoundaries[0], NULL, 0); 
              if(isUnique(fiveThreat, curThreats))
                curThreats.push_back(fiveThreat); 
            }
           break;
          case 3:
            //we have 3 out of the middle four available, now we need to see what threats we can make 
            //with this information
            //between searchLoc and endLoc
            boundaryLoc = searchLoc;
            if(boundaryLoc.decrement(curDir,1) ){
              //check to see if our boundary is empty  
              if(board[boundaryLoc.x][boundaryLoc.y].owner == blank) {
                emptyBoundaries++;
                freeBoundaries.push_back(boundaryLoc);
              }
            }
            boundaryLoc = endLoc;
            if(boundaryLoc.increment(curDir,1) ){
              //check to see if our boundary is empty  
              if(board[boundaryLoc.x][boundaryLoc.y].owner == blank){
                emptyBoundaries++;
                freeBoundaries.push_back(boundaryLoc);
              }
            }
            if(emptyBoundaries==2){
              //create straight four threat
              Threat sfourThreat(sfour,board,myColor);
              //set this threat
            
							if((freeSpaces.size()!=1))
								break; 
							sfourThreat.setValue(freeSpaces[0], NULL, 0); 
              if(isUnique(sfourThreat, curThreats))
                curThreats.push_back(sfourThreat); 
            }
            if(emptyBoundaries==1){
              //create a four threat
              Threat fourThreat1(four,board,myColor);
              Threat fourThreat2(four,board,myColor); 
              //set this threat
							//assert(freeSpaces.size()==1);
							if((freeSpaces.size()!=1))
								break; 

							//assert(freeBoundaries.size()==1);
							fourThreat1.setValue(freeSpaces[0], &(freeBoundaries[0]), 1);
							fourThreat2.setValue(freeBoundaries[0], &(freeSpaces[0]), 1);
              if(isUnique(fourThreat1,curThreats)) 
                curThreats.push_back(fourThreat1);
              if(isUnique(fourThreat2,curThreats)) 
                curThreats.push_back(fourThreat2);             
            }
            break;
          case 5: 
            //NOT POSSIBLE!! 
            break;
          case 2:
            //create some three threats
            boundaryLoc = searchLoc;
            if(boundaryLoc.decrement(curDir,1) ){
              //check to see if our boundary is empty  
              if(board[boundaryLoc.x][boundaryLoc.y].owner == blank){
                freeBoundaries.push_back(boundaryLoc); 
                emptyBoundaries++;
              }
            }
            boundaryLoc = endLoc;
            if(boundaryLoc.increment(curDir,1) ){
              //check to see if our boundary is empty  
              if(board[boundaryLoc.x][boundaryLoc.y].owner == blank){
                freeBoundaries.push_back(boundaryLoc);
                emptyBoundaries++;
              }
            }
            if(emptyBoundaries==2){
							if(freeSpaces.size()!=2)
								break;
							//we can make some threes, 
              // we just need to figure out 
              // what kind of three threat we are making...
              // borken three or three?
              //can create two threats, always. A broken three, or a straight three 
              bool adjacent; 
              threatSpaces.push_back(freeSpaces[0]); 
              for(unsigned int i = 0; i< threatSpaces.size(); ++i){
                adjacent = false; 
                for(unsigned int j = 0; j < threatSpaces.size(); ++j){
                  if(j!=i){
                    if(threatSpaces[i].movesFrom(curDir, threatSpaces[j]) ==1){
                      adjacent = true;
                    }
                  }
                }
                if(adjacent ==false)
                  break;
              }
              if(adjacent ==false){
                //we have our broken three if adjacent is false
                //and since each three situation can create two threats, we 
                Threat brokenThree(bthree, board,myColor); 
                Coordinate threats[3] = {freeSpaces[1],freeBoundaries[0],freeBoundaries[1]}; 
                brokenThree.setValue(freeSpaces[0],threats, 3); 
                if(isUnique(brokenThree,curThreats)) 
                  curThreats.push_back(brokenThree);
              }
              else{
                ThreatType threeThreat = three;
                Threat three(threeThreat,board,myColor); 
                Coordinate threats[3] = {freeSpaces[1],freeBoundaries[0],freeBoundaries[1]}; 
                three.setValue(freeSpaces[0],threats, 3); 
                if(isUnique(three,curThreats))
                  curThreats.push_back(three);
              }
              threatSpaces.pop_back();
              //found what one of the threats will be, see what this threat will be
              threatSpaces.push_back(freeSpaces[1]); 
              for(unsigned int i = 0; i< threatSpaces.size(); ++i){
                adjacent = false; 
                for(unsigned int j = 0; j < threatSpaces.size(); ++j){
                  if(j!=i){
                    if(threatSpaces[i].movesFrom(curDir, threatSpaces[j]) ==1){
                      adjacent = true;
                    }
                  }
                }
                if(adjacent ==false)
                  break;
              }
              if(adjacent ==false){
                //we have our broken three if adjacent is false
                //and since each three situation can create two threats, we 
                Threat brokenThree(bthree,board,myColor); 
                Coordinate threats[3] = {freeSpaces[0],freeBoundaries[0],freeBoundaries[1]}; 
                brokenThree.setValue(freeSpaces[1],threats, 3); 
                if(isUnique(brokenThree,curThreats)) 
                  curThreats.push_back(brokenThree);
              }
              else{
                ThreatType threeThreat = three; 
                Threat three(threeThreat,board,myColor); 
                Coordinate threats[3] = {freeSpaces[0],freeBoundaries[0],freeBoundaries[1]}; 
                three.setValue(freeSpaces[1],threats, 3); 
                if(isUnique(three,curThreats))
                  curThreats.push_back(three);
              }

            }
            break;
        }
        
      }
      if(searchLoc==curMove)
        dirDone = true;

      searchLoc.increment(curDir,1);
    }

  }
  
}


bool Threat::isNewThreat(Threat &discoveredThreat){
  for( unsigned int i =0; i < totalThreats.size(); ++i){
    if((*totalThreats[i])==discoveredThreat)
      return false;
  }
  return true;
}
