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

        cout << "Message" << problem.msg << endl;
        
    }


};