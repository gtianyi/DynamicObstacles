/* CS980 Reinforcement Learning Project 2017Fall
    File:        main.cpp
    Team:        AlphaGroup
    author:      Tianyi Gu
    description: main funciton
    Date:        10 / 26 / 2017
  */
#include <stdio.h>
#include <sys/socket.h>
#include <cstdlib>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <arpa/inet.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <climits>
#include "struct.hpp"
#include "parser.hpp"
#include "rl.hpp"
using namespace std;

#define RUN_PORT 4000
#define TRAIN_PORT 3000

int createSocket(int port){
    int sock = 0;
    struct sockaddr_in address;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock =  socket(AF_INET, SOCK_STREAM,  0)) <  0)
    {
        printf("\n Socket creation error \n");
        return - 1;
    }

    memset( & serv_addr, '0',  sizeof(serv_addr));

    serv_addr.sin_family =  AF_INET;
    serv_addr.sin_port =  htons(port);

    //  Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET,  "127.0.0.1",& serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address / Address not supported \n");
        return -1;
    }

    if (connect(sock,  (struct sockaddr *) & serv_addr,  sizeof(serv_addr)) <  0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    return sock;
}



int main(int argc,  char *argv[])
{
    int trainSocket = createSocket(TRAIN_PORT);
    int runSocket = createSocket(RUN_PORT);
    if(trainSocket == -1 || runSocket == -1) return -1;

    auto paramenters = parseArguments(argc, argv);
    auto problem = getProblemFromSocket(runSocket);
    // this problem2 is just use to avoid crash
    // because of the socket simulator modual need this
    // auto problem2 = getProblemFromSocket(trainSocket);
    Solver solver(paramenters, problem);
    solver.run(runSocket, trainSocket);
    return 0;
}
