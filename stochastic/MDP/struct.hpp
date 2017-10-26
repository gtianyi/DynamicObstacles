/* CS980 Reinforcement Learning Project 2017Fall
   File:        struct.hpp
   Team:        AlphaGroup
   author:      Tianyi Gu
   description: structs
   Date:        10/26/2017
*/ 

#include <vector>
// using namespace std;

struct Position{
    int x;
    int y;
    Position(int _x,int _y):x(_x),y(_y){}
    Position(){}
};


struct AlgParameters{
    string alg = "qlearning";
    double gamma = 1.0;
    int simNum = 0;
};

struct Problem{
    int num_row;
    int num_column;
    int num_staticObs;
    int num_dynamicObs;
    vector<Position>  staticObs;
    vector<Position>  dynamicObs;
    Position start;
    Position goal;
};
        

class ActionNode{
public:
ActionNode(int _id):aid(_id){}
  int aid;
  vector<pair<int, double> > ns;
};

class StateNode{
public:
StateNode(int _id,double _reward,int _isterminal):
  sid(_id),reward(_reward),isterminal(_isterminal),
      u(0),prevu(0),besta(-1){}
  int sid;
  double reward;
  int isterminal;
  double u;
  double prevu;
  int besta;
  vector<ActionNode*> actionList;
};
