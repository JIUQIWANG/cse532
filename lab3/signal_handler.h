#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include <ace/ACE.h>
#include <ace/Event_Handler.h>
#include <signal.h>
#include <atomic>
#include <iostream>

class QuitFlags{
public:
	static void set_quit_flag(){
		quit_flag.store(true);
	}
	static bool is_interrupted() {return interrupt_flag.load();}
	static bool is_quit(){return quit_flag.load();};
	static void interrupt() {interrupt_flag.store(true);}

	static std::atomic<bool> quit_flag;
	static std::atomic<bool> interrupt_flag;
};

class SignalHandler: public ACE_Event_Handler{
public:
    virtual int handle_signal(int signum, siginfo_t* t, ucontext_t* c){
        if(signum == SIGINT){
			QuitFlags::set_quit_flag();
			return -1;
        }
       return 0;
    }
};
#endif
