/* CS980 Reinforcement Learning Project 2017Fall
   File:        parser.hpp
   Team:        AlphaGroup
   author:      Tianyi Gu
   description: parser function
   Date:        10/26/2017
*/
#include "cxxopts.hpp"
#include <vector>
#include "struct.hpp"

// #include "rl.hpp"

AlgParameters parseArguments(int argc, char *argv[]) {
    AlgParameters p;
    //clang - format off
    cxxopts::Options options("RL",  "Algorithm module");
    options.add_options()
            ("a, algorithm", "algorithm name", cxxopts::value(p.alg))
            ("g, gamma", "discoount factor", cxxopts::value(p.gamma))
            ("n, num", "number of episold", cxxopts::value(p.simNum));
    //clang - format on
    options.parse(argc, argv);
    return p;
}

Problem initializeProblem(int sock){
    string startMsg = "start";
    send(sock, startMsg.c_str(), startMsg.length(), 0);
    cout <<  "start message sent..." <<  endl;
    char buffer[1024] =  {0};
    read(sock, buffer, 1024);
    cout << "received: " << buffer << endl;
   
    cout << "initializing problem..." << endl;
    Problem prob;
    
    stringstream ss(buffer);
    ss >> prob.num_row;
    ss >> prob.num_column;
    ss >> prob.num_staticObs;
    ss >> prob.num_dynamicObs;
    for (int i = 0; i < prob.num_staticObs; i++){
        Position p;
        ss >> p.x;
        ss >> p.y;
        prob.staticObs.push_back(p);
    }
    for (int i = 0; i < prob.num_dynamicObs; i++){
        Position p;
        ss >> p.x;
        ss >> p.y;
        prob.dynamicObs.push_back(p);
    }
    ss >> prob.start.x;
    ss >> prob.start.y;
    ss >> prob.goal.x;
    ss >> prob.goal.y;
    return prob;
}

    
   
	
    // if(_alg=="vi"){		
    //   value_iteration myvi(_gamma,_epsilon,starts,stateList,_upperU);
    //   myvi.run();
    // }
    // else if(_alg=="rtdp"){
    //   rtdp myrtdp(_gamma,_simulationNum,starts,stateList,_upperU);
    //   myrtdp.run();
    // }

