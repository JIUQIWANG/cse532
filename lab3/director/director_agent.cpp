#include "director_agent.h"

using namespace std;

DirectorAgent::~DirectorAgent(){
    acceptor.close();
    stream.close();
}

int DirectorAgent::open(char **argv){
    ACE_INET_Addr remote_addr;
    unsigned short local_port;
    string addr_str = string(argv[2]) + ':' + string(argv[1]);
    remote_addr.string_to_addr(addr_str.c_str());
    cout << "Producer address: "<<addr_str<<endl;
    
    //connect to Producer
    ACE_SOCK_Stream stream;
    ACE_SOCK_Connector connector;
    if(connector.connect(stream, remote_addr) < 0){
	cerr << "Can not connect to Producer!" << endl;
	return returnType::E_CONNECTION;
    }

    if(sendPlayList(director, stream, local_port) < 0){
	cerr << "Can not connect to Producer" << endl;
	return returnType::E_CONNECTION;
    }

    //open acceptor
    if(initializeAcceptor(acceptor, local_port) < 0){
	cerr << "Can not open acceptor!" << endl;
	return returnType::E_CONNECTION;
    }
    cout << "Local acceptor opened, port:" << local_port << endl;
    return returnType::SUCCESS;
}

int DirectorAgent::run(){
    SignalHandler* sighandler;
    ACE_NEW_RETURN(sighandler, SignalHandler(), returnType::E_MEMORY);
//	Liveness_sender* liveness_sender;
//	ACE_NEW_RETURN(liveness_sender, Liveness_sender(remote_addr, director, local_port), returnType::EMEMORY);EMEMORY
    const ACE_Time_Value report_interval(1,0);

    if(ACE_Reactor::instance()->register_handler(SIGINT, sighandler) < 0){
	cerr << "DirectorAgent::run(): failed to register to reactor" << endl;
	return returnType::E_REACTOR;
    }
	
    //ACE_Reactor::instance()->schedule_timer(liveness_sender, 0, report_interval, report_interval);

    while(true){
	if(SignalHandler::is_interrupted()) {
	    cout << "closed" << endl << flush;
	    closeConnection();
	    break;
	}
	ACE_Reactor::instance()->handle_events();
    }
    return returnType::SUCCESS;
}

void DirectorAgent::closeConnection(){
    
}
