#include <iostream>
#include "server_acceptor.h"
#include <ace/Reactor.h>
#include "../signal_handler.h"
#include <mutex>
#include <ace/SOCK_Stream.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Connector.h>
using namespace std;

enum returnType{SUCCESS,E_ARGUMENT,E_REACTOR,E_CONNECTION,E_MEMORY, E_OTHER};

void processSignal(ACE_Proactor* proactor, const unsigned short port){
    SignalHandler *signalHandler = 0;
    ACE_NEW_NORETURN(signalHandler, SignalHandler());
    if(!signalHandler)
        return;
    unique_ptr<SignalHandler> guard_signal(signalHandler);
    if(ACE_Reactor::instance()->register_handler(SIGINT, signalHandler) < 0){
        cerr << "Can not register signal handler" << endl;
        return;
    }
	ACE_Time_Value timeout(0,100);
    string close_str("close");
    while(true){
        if(QuitFlags::is_quit()){
            break;
        }
        ACE_Reactor::instance()->handle_events(timeout);
    }

    guard_signal.release();
}

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

    if(acceptor->open(local_addr, 0, false, 5, 1, ACE_Proactor::instance()) < 0){
        cerr << "Can not open acceptor"<<endl;
        return returnType::E_CONNECTION;
    }

    thread t(processSignal, ACE_Proactor::instance(), port);
    thread_guard t_guard(t);

	ACE_Time_Value timeout(0,100);
    while(true) {
        if(QuitFlags::is_quit())
            break;
        ACE_Proactor::instance()->handle_events(timeout);
    }
    guard_acceptor.release();

    return returnType::SUCCESS;
}
