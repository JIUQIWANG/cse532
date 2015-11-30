#ifndef PRODUCER_H
#define PRODUCER_H
#include <memory>
#include <string>
#include <exception>
#include <ace/Thread_Manager.h>
#include <ace/Time_Value.h>

#include "../sender.h"
#include "producer_acceptor.h"

class QuitTimer: public ACE_Event_Handler{
public:
    QuitTimer(std::shared_ptr<PlayList> playlist_): playlist(playlist_){}
    virtual int handle_timeout(const ACE_Time_Value &current_time, const void *act=0);
private:
    std::shared_ptr<PlayList> playlist;
};

class Producer: public ACE_Event_Handler{
public:
    Producer(const unsigned short port_, ACE_Reactor* reactor_=ACE_Reactor::instance());

    //handle keyboard interaction
    virtual int handle_input(ACE_HANDLE=ACE_INVALID_HANDLE);

    int handleKeyboard(const std::string& str);

    //when shutting down, notify all connected directors
    int close();
private:
    const unsigned short port;
    std::shared_ptr<PlayList> playlist;

    //unique_addr keeps tracking of individual connections
    ProducerAcceptor* acceptor;
    ACE_Reactor* reactor;

    //quit_flag is set by "quit" command. When set, the program will quit once the play list empty
    bool quit_flag;
    static const ACE_Time_Value check_interval;
};
#endif
