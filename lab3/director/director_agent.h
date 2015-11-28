#ifndef DIRECTOR_AGENT_H
#define DIRECTOR_AGENT_H

#include "director_acceptor.h"
#include "../common.h"
#include "../signal_handler.h"
#include <iostream>

class DirectorAgent{
public:
	DirectorAgent(std::shared_ptr<Director> director_):
		local_port(0), director(director_), acceptor(director_), stream(), connector(){}

	int open(char **argv);
	int run();
	~DirectorAgent();

private:
	int sendPlayList();
	int initializeAcceptor();
	void closeConnection();

	unsigned short local_port;
	std::shared_ptr<Director> director;
	DirectorAcceptor acceptor;
	ACE_SOCK_Stream stream;
	ACE_SOCK_Connector connector;
};
#endif
