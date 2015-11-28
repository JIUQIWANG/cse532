#ifndef DIRECTOR_AGENT_H
#define DIRECTOR_AGENT_H

#include "director_acceptor.h"
#include "../common.h"
#include "../signal_handler.h"
#include <iostream>

class DirectorAgent{
public:
DirectorAgent(std::shared_ptr<Director> director_):
    director(director_), acceptor(director_), stream(), connector(){}

    int open(char **argv);
    int run();
    ~DirectorAgent();
    
private:
    void closeConnection();
    std::shared_ptr<Director> director;
    DirectorAcceptor acceptor;
    ACE_SOCK_Stream stream;
    ACE_SOCK_Connector connector;
};
#endif
