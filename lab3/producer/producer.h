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

    int handleKeyboard(const std::string& str);
    //when shutting down, notify all connected directors
    void close();
private:
    const unsigned short port;
    std::shared_ptr<PlayList> playlist;

    ProducerAcceptor acceptor;
    ACE_Connector<OutputHandler, ACE_SOCK_Connector> connector;

    ACE_Reactor* reactor;
};
#endif
