#include <iostream>
#include <ace/ACE.h>
#include "producer.h"

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
    try{
	Producer producer(port);
	ACE_Reactor::instance()->run_event_loop();
    }catch(runtime_error e){
	cerr << e.what() << endl;
	return returnType::EOTHER;
    }

    return returnType::SUCCESS;
}
