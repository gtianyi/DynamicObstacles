/* CS980 Reinforcement Learning Project 2017Fall
   File:        struct.hpp
   Team:        AlphaGroup
   author:      Tianyi Gu
   description: structs
   Date:        10/26/2017
*/ 

#include <vector>
using namespace std;

class Position{
  public:
    int x;
    int y;
    Position(int _x,int _y):x(_x),y(_y){}
    Position(){}
    bool operator == (const Position& other){
        return x == other.x && y == other.y;
    }
};


struct AlgParameters{
    string alg = "random";
    double gamma = 1.0;
    int simNum = 10;
    int c = 0.5;
    int depth = 10;
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

string msg2str(Message msg){
    string str =  to_string(msg.agentPos.x) +   " " +
            to_string(msg.agentPos.y) +   " ";
    for (const auto &  obs: msg.dynamicObs){
        str +=   to_string(obs.x) +   " " +
                to_string(obs.y) +   " ";
    }
    return str;
}

Message str2msg(string str){
    Message msg;
    stringstream ss(str);
    ss >>  msg.agentPos.x;
    ss >>  msg.agentPos.y;
    int temp;
    while(ss >> temp){
        Position p;
        p.x = temp;
        ss >>  p.y;
        msg.dynamicObs.push_back(p);
    }
    return msg;
}

class Node: public State{
  public:
    string stateStr;
    Message stateMsg;
    bool isTreeNode = false;
    int ns = 0;
    int nsa[5] = {0};
    int qsa[5] = {0};
    bool isTerminal = false;
    Node(bool isCollide){
        reward = isCollide ? -1000 : -1;
        isTerminal = isCollide;
    }
    void setStateByStr(string str){
        stateStr = str;
        stateMsg = str2msg(str);
    }
};
