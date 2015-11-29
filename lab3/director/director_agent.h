#ifndef DIRECTOR_AGENT_H
#define DIRECTOR_AGENT_H

#include "director_acceptor.h"
#include "../common.h"
#include "../signal_handler.h"
#include <iostream>

class DirectorAgent{
public:
	DirectorAgent(int argc, char** argv);

	int open(char **argv);
	int run();
	~DirectorAgent();

private:
	int sendPlayList();
	int initializeAcceptor();
	void closeConnection();

	unsigned short local_port;
	std::shared_ptr<Director> director;
	DirectorAcceptor* acceptor;
	ACE_SOCK_Stream stream;
	ACE_SOCK_Connector connector;
	ACE_INET_Addr remote_addr;
};
#endif
