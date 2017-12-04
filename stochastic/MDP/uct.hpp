/* CS980 Reinforcement Learning Project 2017Fall
   File:        uct.hpp
   Team:        AlphaGroup
   author:      Tianyi Gu
   description: UCT algorithms
   Date:        12 / 2 / 2017
*/ 
 
#include <unordered_map>
#include"../../deterministic/a-star/probcal.hpp"
#include <cfloat>
#include <math.h>

using namespace std;

class UCT{
public:
    UCT(AlgParameters p, Message prob):
            parameter(p), problem(prob){};
  
    int selectAction(string s){
        int sNum = 0;
        root = setNode(s);
        while(sNum < parameter.simNum){
            sNum++;
            simulate(root, parameter.depth);
        }
        return root->bestAction;
    }
    
private:
    unordered_map<string, shared_ptr<Node>> treeNodes;
    AlgParameters parameter;
    Message problem;
    shared_ptr<Node> root;

    shared_ptr<Node> setNode(string s){
        if (treeNodes.find(s) == treeNodes.end()){
            return createNode(s);
        }
        else{
            return treeNodes[s];
        }
    }
    
    shared_ptr<Node> createNode(string stateStr){ 
        bool isCollide = checkIsCollision(stateStr);
        shared_ptr<Node> n = make_shared<Node>(isCollide);
        n->setStateByStr(stateStr);
        setGoalInfo(n);
        treeNodes[stateStr] = n;
        return n;
    }

    bool checkIsCollision(string stateStr){
        Message stateMsg =  str2msg(stateStr);
        if(isAgentOutBound(stateMsg) ||
           isAgentCollideWithDynamicObs(stateMsg) ||
           isAgentCollideWithStaticObs(stateMsg))
        {
            return true;
        }       
        return false;
    }

    bool isAgentOutBound(Message stateMsg){
        if(stateMsg.agentPos.x <  0 ||
           stateMsg.agentPos.y <  0 ||
           stateMsg.agentPos.x >=  problem.num_column ||
           stateMsg.agentPos.y >=  problem.num_column){
            return true;
        }
        return false;
    }

    bool isAgentCollideWithDynamicObs(Message stateMsg){
        for (const auto &d : stateMsg.dynamicObs){
            if (stateMsg.agentPos == d){
                return true;
            }
        }
        return false;
    }

    bool isAgentCollideWithStaticObs(Message stateMsg){
         for (const auto & p: problem.staticObs){
            if (stateMsg.agentPos ==  p){
                return true;
            }
         }
         return false;
    }

    void setGoalInfo(shared_ptr<Node> s){
        if( !s->isTerminal){
            if(checkIsGoal(s)){
                s->isTerminal = true;
                s->reward = 1000;
            }
        }
    }

    bool checkIsGoal(shared_ptr<Node> s){
        return s->stateMsg.agentPos == problem.goal;
    }

    float simulate(shared_ptr<Node> state, int depth){
        cout << "sim " << depth << endl;
        if (depth == 0){
            return 0;
        }
        if(state->isTerminal){
            return state->reward;
        }
        if ( !state->isTreeNode){
            state->isTreeNode = true;
            state->ns = 1;
            float result = rollout(state, depth);
            return result;
        }
        int action =  selectUCBBestAction(state);
        shared_ptr<Node> nextS = getNextStateByTran(state,
                                                    action);
        float q = state->reward +
                parameter.gamma * simulate(nextS, depth - 1);
        state->ns++;
        state->nsa[action] ++;
        state->qsa[action] = state->qsa[action] +
                (q - state->qsa[action]) /
                (state->nsa[action]);
        return q;
    }

    float rollout(shared_ptr<Node> state, int depth){
        cout <<"rollout " << depth << endl;
        if (depth == 0){
            return 0;
        }
        if (state->isTerminal){
            return state->reward;
        }
        int action = rand() % 5;
        shared_ptr<Node> nextS = getNextStateByTran(state,
                                                    action);
        return state->reward + parameter.gamma * rollout(nextS, depth - 1);
    }

    int selectUCBBestAction(shared_ptr<Node> state){
        int best = -1;
        float maxUCBValue = -FLT_MAX;
        float c = parameter.c;
        for (int i = 0; i < 5; i++){
            float tempUCBValue = FLT_MAX;
            if(state->nsa[i] != 0){
                tempUCBValue = state->qsa[i] +
                        sqrt(log(state->ns / state->nsa[i])) * c;
            }
            if(tempUCBValue > maxUCBValue){
                maxUCBValue = tempUCBValue;
                best = i;
            }
        }
        return best;
    }

    shared_ptr<Node> getNextStateByTran(
        shared_ptr<Node> state,
        int action){
        Message nextSMsg = moveAgent(state->stateMsg, action);
        string nextSStr = msg2str(nextSMsg);
        string agentPosStr = nextSStr.substr(0, 4);
        string dynStr = nextSStr.substr(4);        
        vector<pair<double, string> > trans = getTransProb(dynStr,
                                                           problem.num_row,
                                                           problem.num_column);
        return transToNextState(agentPosStr, trans);
    }

    Message moveAgent(Message stateMsg, int action){
        Message nextSMsg = stateMsg;
        switch(action){
            case 0:
                break;
            case 1:
                nextSMsg.agentPos.x -= 1;
                break;
            case 2:
                nextSMsg.agentPos.x += 1;
                break;
            case 3:
                nextSMsg.agentPos.y += 1;
                break;
            case 4:
                nextSMsg.agentPos.y -= 1;                  
        }
        return nextSMsg;
    }

    shared_ptr<Node> transToNextState(string agentPosStr,
                                      vector<pair<double, string> > trans){
        float curdie = (float)(rand() % 100) / 100.0;
        float accumulateT = 0;
        float paccumulateT = 0;
        string nextDynStr;
        int i = 0;
        for (const auto &t : trans){
            cout << i++ << endl;
            paccumulateT = accumulateT;
            accumulateT += t.first;
            if (curdie <= accumulateT && curdie >= paccumulateT){
                nextDynStr = t.second;
                break;
            }
        }
        replace(nextDynStr.begin(), nextDynStr.end(), ',', ' ');
        string nextStateStr = agentPosStr + nextDynStr;
        return setNode(nextStateStr);
    }
};
