#ifndef SERVER_ACCEPTOR_H
#define SERVER_ACCEPTOR_H
#include <ace/ACE.h>
#include <ace/Acceptor.h>
#include <ace/Event_Handler.h>
#include <ace/Reactor.h>
#include <memory>
#include <ace/SOCK_Acceptor.h>
#include "../socket_handler.h"
class Server_Acceptor: public ACE_Acceptor<SocketHandler, ACE_SOCK_Acceptor>{
public:
    Server_Acceptor(){
        std::cout << "Server_Acceptor " << this << " constructed" << std::endl << std::flush;
    }
    ~Server_Acceptor(){
        std::cout << "Server_Acceptor " << this << " destructed" << std::endl << std::flush;
    }

    virtual int handle_input(ACE_HANDLE fd=ACE_INVALID_HANDLE);
    virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask){
        delete this;
        return 0;
    }
};
#endif
