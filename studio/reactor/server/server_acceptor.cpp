#include "server_acceptor.h"
#include "../socket_handler.h"

using namespace std;

int Server_Acceptor::open(){
    ACE_INET_Addr addr(port);
    if (mAcceptor.open(addr, 1) == -1){
        cerr << "Server_Acceptor::open(): cannot open acceptor"<<endl;
        return -1;
    }
    // register the handler with the reactor
    if (mReactor->register_handler(this,
                                   ACE_Event_Handler::ACCEPT_MASK) == -1) {
        cerr << "Server_Acceptor::open(): cannot register to reactor"<<endl;
        if (mAcceptor.close() == -1)
            cerr << "Server_Acceptor::open(): cannot close acceptor"<<endl;
        return -1;
    }
    return 0;
}


int Server_Acceptor::handle_input(ACE_HANDLE h){
    ACE_INET_Addr client_addr;
    if(mAcceptor.accept(mStream, &client_addr) < 0){
        cerr << "Server_Acceptor::handle_input(): mAcceptor can not accept stream"<<endl;
        return -1;
    }
    SocketHandler* sock_handler;
    ACE_NEW_RETURN(sock_handler, SocketHandler(mStream), -1);
    unique_ptr<SocketHandler> phandler(sock_handler);

    if(mReactor->register_handler(sock_handler, ACE_Event_Handler::READ_MASK) < 0){
        cerr << "Server_Acceptor::handle_input(): can not register socket handler"<<endl;
        return -1;
    }
    phandler.release();
    return 0;
}


int Server_Acceptor::handle_close(ACE_HANDLE h, ACE_Reactor_Mask r){
    if(mStream.close() < 0){
        cerr << "Server_Acceptor::handler_close(): faile to close mStream" << endl;
    }
    if(mAcceptor.close() < 0){
        cerr << "Server_Acceptor::handle_close(): fail to close mAcceptor"<<endl;
    }
    delete this;
    return 0;
}
