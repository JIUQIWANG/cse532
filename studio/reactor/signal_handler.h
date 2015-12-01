#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include <ace/ACE.h>
#include <ace/Event_Handler.h>
#include <ace/Reactor.h>
#include <signal.h>

class SignalHandler{
public:
    SignalHandler();
    virtual int handle_signal(int signum, signal_t* t, ucontext_t* c){
	if(signum == SIGINT){
	    interrupted = true;
	    printf("interrupted!\n");
	}
	return 0;
    }
private:
    static bool interrupted;
};

bool SignalHandler::interrupted = false;

#endif
