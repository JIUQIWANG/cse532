#ifndef PRODUCER_H
#define PRODUCER_H
#include <memory>
#include <string>
#include <exception>
#include "producer_ace.h"

class Producer: public ACE_Event_Handler{
public:
    enum commandType{
	PLAY,
	STOP,
	EXIT
    };
    Producer(const unsigned short port_, ACE_Reactor* reactor_=ACE_Reactor::instance());

    //handle keyboard interaction
    virtual int handle_input(ACE_HANDLE=ACE_INVALID_HANDLE);
    int open();

    int handleKeyboard(const std::string& str);
    //when shutting down, notify all connected directors
    void close();
private:
    const unsigned short port;
    //use shared_ptr to avoid memory leak
    std::shared_ptr<PlayList> playlist;
    std::shared_ptr<ProducerAcceptor > acceptor;
    ACE_Reactor* reactor;
    std::shared_ptr<ACE_Connector<ProducerOutputHandler, ACE_SOCK_Connector> > connector;
};
#endif
