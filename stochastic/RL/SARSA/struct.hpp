/* CS980 Reinforcement Learning Project 2017Fall
   File:        struct.hpp
   Team:        AlphaGroup
   author:      Tianyi Gu
   description: structs
   Date:        10/26/2017
*/ 

#include <vector>
using namespace std;

struct Position{
    int x;
    int y;
    Position(int _x,int _y):x(_x),y(_y){}
    Position(){}
};


struct AlgParameters{
    string alg = "random";
    double gamma = 1.0;
    int simNum = 100;
};

struct Message{
    string msg;
    int num_row;
    int num_column;
    int num_staticObs;
    int num_dynamicObs;
    vector<Position>  staticObs;
    vector<Position>  dynamicObs;
    Position agentPos;
    Position goal;
};

class State{
  public:
    double reward = -1;
    int bestAction = 0;
    double bestQ;
    vector<double> Q = {0, 0, 0, 0, 0};
};
