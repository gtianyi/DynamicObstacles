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
#include <unistd.h>
#include <climits>
#include "parser.hpp"
using namespace std;

#define PORT 3000
int sock = 0;

void createSocketReturnID(){
    struct sockaddr_in address;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock =  socket(AF_INET, SOCK_STREAM,  0)) <  0)
    {
        printf("\n Socket creation error \n");
        sock = -1;
        return;
    }

    memset( & serv_addr, '0',  sizeof(serv_addr));

    serv_addr.sin_family =  AF_INET;
    serv_addr.sin_port =  htons(PORT);

    //  Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET,  "127.0.0.1",& serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address / Address not supported \n");
        sock = -1;
        return;
    }

    if (connect(sock,  (struct sockaddr *) & serv_addr,  sizeof(serv_addr)) <  0)
    {
        printf("\nConnection Failed \n");
        sock = -1;
        return;
    }
}



int main(int argc,  char *argv[])
{
    createSocketReturnID();
    if(sock == -1) return -1;

    auto paramenters = parseArguments(argc, argv);
    auto problem = initializeProblem(sock);
    return 0;
}
