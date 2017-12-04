/* CS980 Reinforcement Learning Project 2017Fall
   File:        rl.hpp
   Team:        AlphaGroup
   author:      Tianyi Gu
   description: RL algorithms
   Date:        12 / 2 / 2017
*/ 
 
#include <unordered_map>
#include "uct.hpp"

using namespace std;

class Solver{
public:
    Solver(AlgParameters p,  Message prob):
            parameter(p), problem(prob){
        initializeProblem();
    };

    void run(int runSocket, int trainSocket){
         if(parameter.alg == "qlearning" ||
            parameter.alg == "sarsa"){
             training(trainSocket);
             runOffLineApproach(runSocket);
         }
         else{
             runOnlineApproach(runSocket, trainSocket);
         }
    }
    
private:
    unordered_map<string, shared_ptr<State>> states;
    AlgParameters parameter;
    Message problem;
    int trainSock = 0;
    int runSock = 0;
    shared_ptr<State> start;
    
    Message trainProblem;
    shared_ptr<State> trainStart;

    void initializeProblem(){
        problem.msg = msg2str(problem);
        start = make_shared<State>();
        states[problem.msg] = start;
    }

     void training(int _sock){
        cout << "training now...  " << endl;
        trainSock = _sock;
        createTrainProblem();
        if(parameter.alg=="random"){
            randomAction();
        }
        else if(parameter.alg == "qlearning"){
            qlearning();
        }
        else if(parameter.alg == "sarsa"){
            // sara();
        }
        else if(parameter.alg == "uct"){
            // no training for online methods
        }
        cout << "finish training ! ! !" << endl;
        return ;
    }

    void createTrainProblem(){
        trainProblem =  getProblemFromSocket(trainSock);
        trainProblem.msg = to_string(trainProblem.agentPos.x) + " " +
                to_string(trainProblem.agentPos.y) + " ";
        for (const auto &obs : trainProblem.dynamicObs){
            trainProblem.msg += to_string(obs.x) + " " +
                    to_string(obs.y) + " ";
        }
        
        if(states.find(trainProblem.msg) == states.end()){
            trainStart = make_shared<State>();
            states[trainProblem.msg] = trainStart;
        }
        else{
            trainStart = states[trainProblem.msg];
        }
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
        s->bestAction = bestActions[a];
        s->bestQ = *it;
        return bestActions[a];
    }
    
    void qlearning(){
        int numOfEpisode = 0;
        while(numOfEpisode < parameter.simNum){
            numOfEpisode++;
            string msg = "";
            shared_ptr<State> curState = trainStart;
            cout << "Qvalue: "
                 << curState->Q[0] <<  " "
                 << curState->Q[1] <<  " "
                 << curState->Q[2] <<  " "
                 << curState->Q[3] <<  " "
                 << curState->Q[4] <<  endl;
            // cout << "bestQ: " << curState->bestQ << endl;
            double step = 1.0;
            while(msg != "lose" && msg != "win"){
                int a = chooseAction(curState);
                // cout << "Qvalue: "
                 // << curState->Q[0] <<  " "
                 // << curState->Q[1] <<  " "
                 // << curState->Q[2] <<  " "
                 // << curState->Q[3] <<  " "
                 // << curState->Q[4] <<  endl;
                // cout << "bestQ: " << curState->bestQ << endl;
                // cout << "bestAction: " << curState->bestAction << endl;
                // cout << "choosed action: " << a << endl;
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
                    nextQ = 1000;
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

    void runOffLineApproach(int _sock){
        cout << "running now..." << endl;
        runSock = _sock;
        shared_ptr<State> curState = start;
        int steps = 0;
        while(true){
            // cout << "Qvalue: "
            //      << curState->Q[0] <<  " "
            //      << curState->Q[1] <<  " "
            //      << curState->Q[2] <<  " "
            //      << curState->Q[3] <<  " "
            //      << curState->Q[4] <<  endl;
            int a = 0;
            if(curState != nullptr) a = curState->bestAction;
            else a = rand() % 5;
            // cout << "bestQ: " << curState->bestQ << endl;
            // cout << "bestAction: " << curState->bestAction << endl;
            // cout << "choosed action: " << a << endl;
            string msg = getStateByActionViaSocket(runSock, to_string(a));
            if (msg == "lose" || msg == "win"){
                cout << msg << endl;
                cout << steps << endl;
                break;
            }
            if(states.find(msg) != states.end())
                curState = states[msg];
            else {
                curState = nullptr;
                cout << "new state" << endl;
            }
            steps++;
        }
    }

    void runOnlineApproach(int runSocket, int trainSocket){
        cout << "running now..." << endl;
        if(parameter.alg == "uct" ){
             uctOnline(runSocket);
         }
        else if(parameter.alg == "sLSPI"){
            sLSPI(runSocket, trainSocket);
         }
    }

    void uctOnline(int runSocket){
        runSock = runSocket;
        shared_ptr<UCT> myUCT = make_shared<UCT>(parameter, problem);
        string stateStr = problem.msg;
        int steps = 0;
        while(true){
            int a = myUCT->selectAction(stateStr);
            stateStr = getStateByActionViaSocket(runSock, to_string(a));
            if (stateStr == "lose" || stateStr == "win"){
                cout << stateStr << endl;
                break;
            }
            cout << steps++ << endl;
        }
    }

    void sLSPI(int runSocket, int trainSocket){
        return;
    }

};
