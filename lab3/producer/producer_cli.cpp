#include <iostream>
#include <ace/ACE.h>
#include "producer.h"
#include "../signal_handler.h"

using namespace std;

enum argumentList{A_NAME, A_PORT, A_INVALID};

int main(int argc, char** argv){
	if(argc >= argumentList::A_INVALID){
		cerr << "Usage: "<<argv[argumentList::A_NAME] << "[port]"<<endl;
		return returnType::E_ARGUMENT;
	}
	unsigned short port;
	if(argc == argumentList::A_PORT)
		port = default_port;
	else
		port = std::atoi(argv[argumentList::A_PORT]);

	SignalHandler* sighandler;
	ACE_NEW_RETURN(sighandler, SignalHandler(), returnType::E_MEMORY);

	//we wrap the raw pointer into a smart pointer to ensure that the memory is properly
	//released even if it's not registered to the reactor.
	unique_ptr<SignalHandler> sighandler_guard(sighandler);
	ACE_Reactor::instance()->register_handler(SIGINT, sighandler);

	ACE_Time_Value timeout(0,100);
	try {
		Producer *producer;
		ACE_NEW_RETURN(producer, Producer(port), returnType::E_MEMORY);
		unique_ptr<Producer> producer_guard(producer);
		while (true) {
			ACE_Reactor::instance()->handle_events(timeout);
			if (QuitFlags::quit_flag.load()){
				if(producer->close() < 0)
					throw runtime_error("Failed to quit");
			}
			if(QuitFlags::is_interrupted())
				break;
		}
		producer_guard.release();
	}catch(const runtime_error& e){
		cerr << e.what() << endl;
		return returnType::E_OTHER;
	}

	//The reactor will call handle_close() hook method to release the memory, thus we should
	//release the ownership of the handler to avoid reactor call a deleted object.
	sighandler_guard.release();
	return returnType::SUCCESS;
}
