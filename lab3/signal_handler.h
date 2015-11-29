#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include <ace/ACE.h>
#include <ace/Event_Handler.h>
#include <signal.h>

class SignalHandler: public ACE_Event_Handler{
public:
    SignalHandler(){};
    static inline bool is_interrupted() {return interrupted;}
    static inline void interrupt() {interrupted = true;}
    static inline bool is_quit() {return quit_flag;}
    static inline void set_quit_flag(){quit_flag = true;}
    virtual int handle_signal(int signum, siginfo_t* t, ucontext_t* c){
        if(signum == SIGINT){
            quit_flag = true;
        }
        return -1;
    }
private:
    static bool interrupted;
    static bool quit_flag;
};
#endif
