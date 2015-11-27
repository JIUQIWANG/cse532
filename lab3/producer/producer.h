#ifndef PRODUCER_H
#define PRODUCER_H
#include <memory>
#include <string>
#include <exception>
#include <ace/Thread_Manager.h>
#include <ace/Time_Value.h>

#include "../sender.h"
#include "producer_acceptor.h"

class Producer: public ACE_Event_Handler{
public:
    Producer(const unsigned short port_, ACE_Reactor* reactor_=ACE_Reactor::instance());

    //handle keyboard interaction
    virtual int handle_input(ACE_HANDLE=ACE_INVALID_HANDLE);

    int handleKeyboard(const std::string& str);


    //when shutting down, notify all connected directors
    void close();
private:
    const unsigned short port;
    std::shared_ptr<PlayList> playlist;

    //unique_addr keeps tracking of individual connections
    std::shared_ptr<unique_set> unique_addr;
    ProducerAcceptor acceptor;
    ACE_Reactor* reactor;

    static const ACE_Time_Value check_interval;
};
#endif
