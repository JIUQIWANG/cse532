#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include <ace/ACE.h>
#include <ace/Event_Handler.h>
#include <signal.h>

class SignalHandler: public ACE_Event_Handler{
public:
	SignalHandler(){
	    std::cout << "SignalHandler " << this << " constructed" << std::endl;
	}
	~SignalHandler(){
	    std::cout << "SignalHandler " << this << " destructed" << std::endl;
	}
	static inline bool is_interrupted() {return interrupted;}

	virtual int handle_signal(int signum, siginfo_t* t, ucontext_t* c){
		if(signum == SIGINT){
			interrupted = true;
			printf("interrupted!\n");
		}
		return -1;
	}
	virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask){
		delete this;
		return 0;
	}
private:
	static bool interrupted;
};

bool SignalHandler::interrupted = false;

#endif
