#ifndef CLIENT_H
#define CLIENT_H

#include <ace/ACE.h>
#include <ace/Event_Handler.h>
#include <ace/SOCK_Connector.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Stream.h>
#include <ace/Reactor.h>
#include <string>
#include <vector>
#include <csignal>

class Client: public ACE_Event_Handler{
public:
    Client(int argc, char** argv, const ACE_SOCK_Stream& stream_);

    virtual int handle_timeout(const ACE_Time_Value& value, const void *pvoid);
//    virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask);
private:
    std::vector<std::string> messages;
    const ACE_SOCK_Stream &stream;
    int counter;
};

#endif
