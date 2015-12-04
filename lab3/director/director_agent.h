#ifndef DIRECTOR_AGENT_H
#define DIRECTOR_AGENT_H

#include "director_acceptor.h"
#include "../common.h"
#include "../signal_handler.h"
#include <iostream>

class DirectorAgent{
public:
	enum argumentList{A_NAME, A_PORT, A_ADDRESS, A_MINPLAYER, A_SCRIPT};

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
	//we use a smart point to ensure that the memory of acceptor will be
	//released if exception happens before it's registered to the reactor
	std::unique_ptr<DirectorAcceptor> acceptor_guard;
	ACE_SOCK_Stream stream;
	ACE_SOCK_Connector connector;
	ACE_INET_Addr remote_addr;
	ACE_Time_Value timeout;
};
#endif
