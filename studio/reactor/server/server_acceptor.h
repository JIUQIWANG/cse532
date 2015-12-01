#ifndef SERVER_ACCEPTOR_H
#define SERVER_ACCEPTOR_H
#include <ace/ACE.h>
#include <ace/Event_Handler.h>
#include <ace/Reactor.h>
#include <ace/SOCK_Acceptor.h>

class Server_Acceptor: public ACE_Event_Handler{
public:
Server_Acceptor(const unsigned short port_, ACE_Reactor* reactor_=0): port(port_),
	mReactor(reactor_ == 0 ? ACE_Reactor::instance():reactor_),
	mAcceptor(){}
    
    virtual int open ();
    virtual ACE_HANDLE get_handle()const {return mAcceptor.get_handle();}

    virtual int handle_input(ACE_HANDLE h = ACE_INVALID_HANDLE);
    virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask);
    
private:
    const unsigned short port;
    ACE_Reactor *mReactor;
    ACE_SOCK_Acceptor mAcceptor;
    ACE_SOCK_Stream mStream;
};
#endif
