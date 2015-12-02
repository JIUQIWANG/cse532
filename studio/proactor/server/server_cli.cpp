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
    string close_str("close");
    while(true){
        cout << SignalHandler::is_interrupted() << endl << std::flush;
        if(SignalHandler::is_interrupted()){
            //send a message to local port, force proactor to handle_event
            cout << "c1" << endl << std::flush;
            ACE_INET_Addr local_addr(port, ACE_LOCALHOST);
            ACE_SOCK_Connector connector;
            ACE_SOCK_Stream stream;
            if(connector.connect(stream, local_addr) < 0)
                cout << "processSignal(): can not connect" << endl << std::flush;
            cout << "sending messsage" << endl << std::flush;
            stream.send_n(close_str.c_str(), close_str.size());
            cout << "Message sent" << endl << std::flush << endl;
            break;
        }
        ACE_Reactor::instance()->handle_events();
    }

    cout << "c1" << endl << std::flush;
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

    while(true) {
        if(SignalHandler::is_interrupted())
            break;
        ACE_Proactor::instance()->handle_events();
    }

    cout << "c1" << endl << std::flush;
    guard_acceptor.release();

    return returnType::SUCCESS;
}
