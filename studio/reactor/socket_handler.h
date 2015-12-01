#ifndef __SOCKET_HANDLER_H__
#define __SOCKET_HANDLER_H__

#include <ace/Event_Handler.h>
#include <ace/SOCK_Stream.h>
#include <ace/Time_Value.h>
#include <iostream>

class SocketHandler : public ACE_Event_Handler {
public:
SocketHandler(const unsigned short port_):port(port_);
    virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE);

    SOCK_Stream& getStream(){return mStream;}
Private:
    const unsigned short port;
    ACE_SOCK_Stream mStream;
    char mData;

};

#endif /* __SOCKET_HANDLER_H__ */

