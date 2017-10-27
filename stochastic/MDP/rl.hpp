/* CS980 Reinforcement Learning Project 2017Fall
   File:        rl.hpp
   Team:        AlphaGroup
   author:      Tianyi Gu
   description: RL algorithms
   Date:        10 / 27 / 2017
*/ 
 
#include <unordered_map>

using namespace std;

class Solver{   
private:
    unordered_map<string, shared_ptr<State>> states;
    AlgParameters parameter;
    Message problem;
    int sock = 0;

    void initializeProblem(){
        problem.msg = to_string(problem.agentPos.x) + "  " +
                to_string(problem.agentPos.y) + "  ";
        for (const auto &obs : problem.dynamicObs){
            problem.msg += to_string(obs.x) + " " +
                    to_string(obs.y) + " ";
        }
        shared_ptr<State> start = make_shared<State>(); 
        states[problem.msg] = start;
    }

    void qlearning(){}
    void randomAction(){
        int numOfEpisode = 0;
        while(numOfEpisode < parameter.simNum){
            numOfEpisode++;
            int action = rand() % 4;
            getStateByActionViaSocket(sock, to_string(action));
        }
    }
  
public:    
    Solver(AlgParameters p,  Message prob):
            parameter(p), problem(prob){
        initializeProblem();
    };
  
    void run(int _sock){
        sock = _sock;
        if(parameter.alg=="random"){
            randomAction();
        }
        else if(parameter.alg == "qlearning"){
            qlearning();
        }
        return ;
    }
};
