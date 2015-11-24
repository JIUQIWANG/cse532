#include <iostream>
#include <ace/ACE.h>
#include "producer.h"
#include "../signal_handler.h"

using namespace std;

int main(int argc, char** argv){
    if(argc > 2){
	cerr << "Usage: "<<argv[0] << "[port]"<<endl;
	return returnType::EARGUMENT;
    }
    unsigned short port;
    if(argc == 1)
	port = default_port;
    else
	port = std::atoi(argv[1]);

    SignalHandler sighandler;
    ACE_Reactor::instance()->register_handler(SIGINT, &sighandler);
    try{
	Producer producer(port);
	while(true){
	    if(SignalHandler::is_interrupted()){
		producer.close();
		break;
	    }
	    ACE_Reactor::instance()->handle_events();
	}

    }catch(runtime_error e){
	cerr << e.what() << endl;
	return returnType::EOTHER;
    }

    return returnType::SUCCESS;
}
