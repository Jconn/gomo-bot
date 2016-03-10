
class threat{
  vector <threat> children;
  ThreatType value;
  Loc beginLoc;
  Loc endLoc;
  vector <Loc> gains;
  vector <Loc> costs;
  public:
  Loc(){

  }
  Loc(Loc begin,Loc end,vector<Loc> gainVec,
      vector<Loc> costVec, ThreatType type)
    :
      beginLoc(begin),
      endLoc(end),
      gains(gainVec),
      costs(costVec),
      value(type)
  {}

};
