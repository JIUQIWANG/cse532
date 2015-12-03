#include <iostream>
#include "server_acceptor.h"
#include "../signal_handler.h"
using namespace std;

enum returnType{SUCCESS,E_ARGUMENT,E_REACTOR,E_CONNECTION,E_MEMORY, E_OTHER};

int main(int argc, char** argv){
    if(argc > 1){
        cerr << "Invalid arguments!"<<endl;
        return returnType::E_ARGUMENT;
    }
    cout << argv[0] << endl;
    const unsigned short port = 2000;

    ACE_INET_Addr local_addr(2000);
    Server_Acceptor *acceptor;
    ACE_NEW_RETURN(acceptor, Server_Acceptor(), returnType::E_MEMORY);
    unique_ptr<Server_Acceptor> guard_acceptor(acceptor);

    if(acceptor->open(local_addr) < 0){
        cerr << "Can not open acceptor"<<endl;
        return returnType::E_CONNECTION;
    }

    SignalHandler *signalHandler;
    ACE_NEW_RETURN(signalHandler, SignalHandler(), returnType::E_MEMORY);
    unique_ptr<SignalHandler> guard_signal(signalHandler);
    if(ACE_Reactor::instance()->register_handler(SIGINT, signalHandler) < 0){
        cerr << "Can not register signal handler" << endl;
        return returnType::E_REACTOR;
    }

    while(true){
        if(SignalHandler::is_interrupted()){
            break;
        }
        ACE_Reactor::instance()->handle_events();
    }

    guard_acceptor.release();
    guard_signal.release();
    return returnType::SUCCESS;
}
