#ifndef __SOCKET_HANDLER_H__
#define __SOCKET_HANDLER_H__

#include <ace/Event_Handler.h>
#include <ace/Svc_Handler.h>
#include <ace/SOCK_Stream.h>
#include <ace/Time_Value.h>
#include <iostream>

class SocketHandler : public ACE_Svc_Handler<ACE_SOCK_Stream, ACE_NULL_SYNCH> {
public:
    SocketHandler(){
        std::cout << "SocketHandler " << this << " constructed" << endl;
    }
    ~SocketHandler(){
        std::cout << "SocketHandler " << this << " destructed" << endl;
    }
    virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE);
    virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask){
        delete this;
        return 0;
    }
};

#endif /* __SOCKET_HANDLER_H__ */

