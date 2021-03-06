#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include <ace/ACE.h>
#include <ace/Event_Handler.h>
#include <signal.h>
#include <atomic>
#include "common.h"

class SignalHandler: public ACE_Service_Object{
public:
	SignalHandler(){
		Service_Logger::registerService(this, "SignalHandler");
	    std::cout << "SignalHandler " << this << " constructed" << std::endl;
	}
	~SignalHandler(){
		Service_Logger::unregister(this);
	    std::cout << "SignalHandler " << this << " destructed" << std::endl;
	}
	static inline bool is_interrupted() {return interrupted.load();}

	virtual int handle_signal(int signum, siginfo_t* t, ucontext_t* c){
		if(signum == SIGINT){
			interrupted.store(true);
			printf("interrupted!\n");
		}
		return -1;
	}
	virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask){
		delete this;
		return 0;
	}
private:
	static std::atomic<bool> interrupted;
};

std::atomic<bool> SignalHandler::interrupted(false);

#endif
