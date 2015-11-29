#include <iostream>
#include <ace/ACE.h>
#include "producer.h"
#include "../signal_handler.h"

using namespace std;

int main(int argc, char** argv){
	if(argc > 2){
		cerr << "Usage: "<<argv[0] << "[port]"<<endl;
		return returnType::E_ARGUMENT;
	}
	unsigned short port;
	if(argc == 1)
		port = default_port;
	else
		port = std::atoi(argv[1]);

	SignalHandler* sighandler;
	ACE_NEW_RETURN(sighandler, SignalHandler(), returnType::E_MEMORY);
	ACE_Reactor::instance()->register_handler(SIGINT, sighandler);



	try {
		Producer *producer;
		ACE_NEW_RETURN(producer, Producer(port), returnType::E_MEMORY);
		while (true) {
			if(SignalHandler::is_interrupted())
				break;
			ACE_Reactor::instance()->handle_events();
			if (SignalHandler::is_quit()) {
				if(producer->close() < 0)
					throw runtime_error("Failed to quit");
			}
		}
	}catch(runtime_error e){
		cerr << e.what() << endl;
		return returnType::E_OTHER;
	}

	return returnType::SUCCESS;
}
