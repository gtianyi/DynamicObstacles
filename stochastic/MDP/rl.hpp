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
    int trainSock = 0;
    int runSock = 0;
    shared_ptr<State> start;

    void initializeProblem(){
        problem.msg = to_string(problem.agentPos.x) + "  " +
                to_string(problem.agentPos.y) + "  ";
        for (const auto &obs : problem.dynamicObs){
            problem.msg += to_string(obs.x) + " " +
                    to_string(obs.y) + " ";
        }
        start = make_shared<State>(); 
        states[problem.msg] = start;
    }
   
    void randomAction(){
        int numOfEpisode = 0;
        while(numOfEpisode < parameter.simNum){
            numOfEpisode++;
            int action = rand() % 5;
            getStateByActionViaSocket(trainSock, to_string(action));
        }
    }

    int chooseAction(shared_ptr<State> s){
        auto it = max_element(begin(s->Q), end(s->Q));
        vector<int> bestActions;
        for (int i = 0;i < s->Q.size();i++){
            if(s->Q[i] == *it) bestActions.push_back(i);
        }
        int a = rand() % bestActions.size();
        s->bestAction = a;
        s->bestQ = *it;
        return a;
    }
    
    void qlearning(){
        int numOfEpisode = 0;
        while(numOfEpisode < parameter.simNum){
            numOfEpisode++;
            string msg = "";
            shared_ptr<State> curState = start;
            double step = 1.0;
            while(msg != "lose" && msg != "win"){
                int a = chooseAction(curState);
                msg = getStateByActionViaSocket(trainSock, to_string(a));
                double reward = 0;
                double nextQ = 0;
                shared_ptr<State> nextS; 
                if(msg == "lose"){
                    reward = -1000;
                    nextQ = -1000;
                }
                else if(msg == "win"){
                    reward = 1000;
                    nextQ = -1000;
                }
                else{
                    if(states.find(msg) != states.end()){
                        nextS = states[msg];
                    }
                    else{
                        nextS = make_shared<State>();
                        states[msg] = nextS;
                    }
                    nextQ = nextS->bestQ;
                    reward = nextS->reward;
                }
                curState->Q[a] += 1.0 / step *
                        (reward + parameter.gamma *nextQ
                         - curState->Q[a]);
                curState = nextS;
                step += 1.0;
            }
        }
    }
public:    
    Solver(AlgParameters p,  Message prob):
            parameter(p), problem(prob){
        initializeProblem();
    };
  
    void training(int _sock){
        cout << "training now...  " << endl;
        trainSock = _sock;
        if(parameter.alg=="random"){
            randomAction();
        }
        else if(parameter.alg == "qlearning"){
            qlearning();
        }
        cout << "finish training ! ! !" << endl;
        return ;
    }

     void run(int _sock){
        cout << "running now..." << endl;
        runSock = _sock;
        shared_ptr<State> curState = start;
        while(true){
            int a = 0;
            if(curState != nullptr) a = curState->bestAction;
            else a = rand() % 5;
            string msg = getStateByActionViaSocket(runSock, to_string(a));
            if (msg == "lose" || msg == "win"){
                cout << msg << endl;
                break;
            }
            if(states.find(msg) != states.end()) curState = states[msg];
            else curState = nullptr;
        }
    }
};
