/* CS980 Reinforcement Learning Project 2017Fall
   File:        parser.hpp
   Team:        AlphaGroup
   author:      Tianyi Gu
   description: parser function
   Date:        10/26/2017
*/
#include "cxxopts.hpp"
#include <vector>
using namespace std;

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

string sendRequestAndGetReturn(int sock, string req){
    send(sock, req.c_str(), req.length(), 0);
    cout <<  "Sent message: " << req <<  endl;
    char buffer[1024] =  {0};
    read(sock, buffer, 1024);
    cout << "received: " << buffer << endl;
    return string(buffer);
}

Message getProblemFromSocket(int sock){
    string msg = sendRequestAndGetReturn(sock, "start");
    cout << "initializing problem..." << endl;
    Message prob;
    stringstream ss(msg);
    ss >> prob.num_row;
    ss >> prob.num_column;
    ss >> prob.num_staticObs;
    ss >> prob.num_dynamicObs;
    ss >> prob.agentPos.x;
    ss >> prob.agentPos.y;
    for (int i = 0; i < prob.num_dynamicObs; i++){
        Position p;
        ss >> p.x;
        ss >> p.y;
        prob.dynamicObs.push_back(p);
    }
    for (int i = 0; i < prob.num_staticObs; i++){
        Position p;
        ss >> p.x;
        ss >> p.y;
        prob.staticObs.push_back(p);
    }
    ss >> prob.goal.x;
    ss >> prob.goal.y;
    return prob;
}

string getStateByActionViaSocket(int sock, string action){
    return sendRequestAndGetReturn(sock,action);
    // might need other thing here as well
}


