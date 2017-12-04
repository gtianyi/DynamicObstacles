/* CS980 Reinforcement Learning Project 2017Fall
   File:        rl.hpp
   Team:        AlphaGroup
   author:      Tianyi Gu
   description: RL algorithms
   Date:        10 / 27 / 2017
*/ 
 
#include <unordered_map>
#include <math.h>

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

        cout << "Message" << problem.msg << endl;
        
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

    void sarsalearning(){
        int numOfEpisode = 0;
        while(numOfEpisode < parameter.simNum){
            numOfEpisode++;
            string msg = "";
            shared_ptr<State> curState = start;
            cout << "Qvalue: "
                 << curState->Q[0] <<  " "
                 << curState->Q[1] <<  " "
                 << curState->Q[2] <<  " "
                 << curState->Q[3] <<  " "
                 << curState->Q[4] <<  endl;
            // cout << "bestQ: " << curState->bestQ << endl;
            double step = 1.0;

            int a = chooseAction(curState);
            //chose a action
            //cout << "Action" << a << endl;
            bool statereached = false;
            while(msg != "lose" && msg != "win"){

                msg = getStateByActionViaSocket(trainSock, to_string(a));
                int ap = chooseAction(curState);
                // cout << "Qvalue: "
                 // << curState->Q[0] <<  " "
                 // << curState->Q[1] <<  " "
                 // << curState->Q[2] <<  " "
                 // << curState->Q[3] <<  " "
                 // << curState->Q[4] <<  endl;
                // cout << "bestQ: " << curState->bestQ << endl;
                // cout << "bestAction: " << curState->bestAction << endl;
                // cout << "choosed action: " << a << endl;
                
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
                    nextQ = nextS->Q[ap];
                    reward = nextS->reward;
                }
                curState->Q[a] += 1.0 / step *
                        (reward + parameter.gamma *nextQ
                         - curState->Q[a]);
                curState = nextS;//action also changes
                a=ap;
                step += 1.0;
            }
        }
    }

    
    void qlearning(){
        int numOfEpisode = 0;
        while(numOfEpisode < parameter.simNum){
            numOfEpisode++;
            string msg = "";
            shared_ptr<State> curState = start;//Initialize s
            cout << "Qvalue: "
                 << curState->Q[0] <<  " "
                 << curState->Q[1] <<  " "
                 << curState->Q[2] <<  " "
                 << curState->Q[3] <<  " "
                 << curState->Q[4] <<  endl;
            // cout << "bestQ: " << curState->bestQ << endl;
            double step = 1.0;
            while(msg != "lose" && msg != "win"){
                int a = chooseAction(curState);//choose action
                // cout << "Qvalue: "
                 // << curState->Q[0] <<  " "
                 // << curState->Q[1] <<  " "
                 // << curState->Q[2] <<  " "
                 // << curState->Q[3] <<  " "
                 // << curState->Q[4] <<  endl;
                // cout << "bestQ: " << curState->bestQ << endl;
                // cout << "bestAction: " << curState->bestAction << endl;
                // cout << "choosed action: " << a << endl;
                msg = getStateByActionViaSocket(trainSock, to_string(a));//get action
                cout << "Action" << a << endl;
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
                         - curState->Q[a]);//formula part
                curState = nextS;//new state is assigned to old state
                step += 1.0;
            cout << "Qvalue" << nextQ << endl;    
            
            }


        }
    }
    vector<float> f1;
    vector<float> f2;
    vector<float> Q;
    //float w1,w2,w3,w4;

    float w1 = 1;
    float w2 = 1;
    float w3 = 1;
    float w4 = 5;

    void valuefunction(){
        // problem.msg = to_string(problem.agentPos.x) + "  " +
        //         to_string(problem.agentPos.y) + "  ";
        // for (const auto &obs : problem.dynamicObs){
        //     problem.msg += to_string(obs.x) + " " +
        //             to_string(obs.y) + " ";
        //     // int len = problem.msg.length();
        //     // int arr[12] = {};
        //     // for(int i=0;i<len;i++){
        //     //     char c =  problem.msg.at(i);
        //     //     if(c != " "){
        //     //         arr[i] = c - '0';
        //     //     }
        //     // }
        //     // float f = arr[0] + 10*arr[1] + arr[2] + 10 * arr[3] +
        //     //             arr[4] + 10 * arr[5] + arr[6] + 10 * arr[7] +
        //     //             arr[8] + 10 * arr[9] + arr[10] + 10 * arr[11];
            
        //     // std::cout << "f value is: " << f;
        //     // f1.push_back(f);

        // }
        // start = make_shared<State>(); 
        // states[problem.msg] = start;

        // cout << "Message" << problem.msg << endl;

        // int len = problem.msg.length();

        // cout << "Length" << len << endl;
        //     int arr[12] = {};
        //     for(int i=0;i<len;i++){
        //         char c =  problem.msg.at(i);
        //         if(arr[i] != ' '){
        //             arr[i] = c -'0';
        //         }
        //     }

        //     cout << "arr[0]" << arr[0] << endl;
        //     cout << "arr[1]" << arr[1] << endl;
        //     cout << "arr[2]" << arr[2] << endl;
        //     cout << "arr[3]" << arr[3] << endl;
            
        //     float f = arr[0] + 10*arr[1] + arr[2] + 10 * arr[3] +
        //                 arr[4] + 10 * arr[5] + arr[6] + 10 * arr[7] +
        //                 arr[8] + 10 * arr[9] + arr[10] + 10 * arr[11];



        //     float g = arr[0] * (pow(2,0)) + arr[1] * (pow(2,0)) + 
        //             arr[2] * (pow(2,1)) + arr[3] * (pow(2,1)) +
        //                 arr[4] * (pow(2,2)) + arr[5] * (pow(2,2)) + 
        //                 arr[6] * (pow(2,3))+ arr[7] * (pow(2,3)) +
        //                 arr[8] * (pow(2,4)) + arr[9] * (pow(2,4)) + 
        //                 arr[10] * (pow(2,5)) + arr[11] * (pow(2,5));
            
        //     std::cout << "f value is: " << f << endl;
        //     std::cout << "g value is: " << g << endl;
        //     std::cout << "w value is: " << w1 << endl;


        //     //float h = pow(10,a);// which a to consider

        //     float qv = (w1*f) + (w2*g) + w4;

        //     std::cout << "w value is: " << w1 << endl;


        //     f1.push_back(f);// we dont need this
        //     f2.push_back(g);//we dont need this
        //     //f2.push_back(qv);



        // //cout << "Message" << problem.msg << endl;

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
        else if(parameter.alg == "sarsa"){
            // sara();
            sarsalearning();
        }
        else if(parameter.alg == "approx"){
            // sara();
            valuefunction();
        }
        else{
            cout<<"no algorithm matched!!!!!"<<endl;
        }
        //cout << "finish training ! ! !" << endl;
        return ;
    }

     void run(int _sock){
        //cout << "running now..." << endl;
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
            if(states.find(msg) != states.end()) curState = states[msg];
            else curState = nullptr;
            steps++;
        }
    }
};
