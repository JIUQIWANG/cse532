#ifndef SERVER_ACCEPTOR_H
#define SERVER_ACCEPTOR_H
#include <ace/ACE.h>
#include <ace/Acceptor.h>
#include <ace/Event_Handler.h>
#include <ace/Reactor.h>
#include <memory>
#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Stream.h>
#include "../interceptor.h"

class SocketHandler : public ACE_Svc_Handler<ACE_SOCK_Stream, ACE_NULL_SYNCH> {
public:
    SocketHandler(){
        std::cout << "SocketHandler " << this << " constructed" << std::endl;
    }
    SocketHandler(std::shared_ptr<EndPoint> endpoint_): endpoint(endpoint_){
        std::cout << "SocketHandler " << this << " constructed" << std::endl;
    }
    ~SocketHandler(){
        std::cout << "SocketHandler " << this << " destructed" << std::endl;
    }
    virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE);
    virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask){
        delete this;
        return 0;
    }
private:
    std::shared_ptr<EndPoint> endpoint;
};

class Server_Acceptor: public ACE_Acceptor<SocketHandler, ACE_SOCK_Acceptor>{
public:
    Server_Acceptor(std::shared_ptr<EndPoint> endpoint_): endpoint(endpoint_){
        std::cout << "Server_Acceptor " << this << " constructed" << std::endl << std::flush;
    }
    ~Server_Acceptor(){
        std::cout << "Server_Acceptor " << this << " destructed" << std::endl << std::flush;
    }

    virtual int make_svc_handler(SocketHandler *&sh);
    virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask){
        delete this;
        return 0;
    }
private:
    std::shared_ptr<EndPoint> endpoint;
};
#endif
