#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include <ace/ACE.h>
#include <ace/Event_Handler.h>
#include <signal.h>
#include <atomic>

class SignalHandler: public ACE_Event_Handler{
public:
    static inline bool is_interrupted() {return interrupt_flag.load();}
    static inline void interrupt() {interrupt_flag.store(true);}
    static inline bool is_quit() {return quit_flag.load();}
    static inline void set_quit_flag(){quit_flag.store(true);}
    virtual int handle_signal(int signum, siginfo_t* t, ucontext_t* c){
        if(signum == SIGINT){
            quit_flag.store(true);
        }
        return -1;
    }
private:
    static std::atomic<bool> interrupt_flag;
    static std::atomic<bool> quit_flag;
};
#endif
