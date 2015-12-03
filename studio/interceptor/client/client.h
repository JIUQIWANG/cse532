#ifndef CLIENT_H
#define CLIENT_H

#include <ace/ACE.h>
#include <ace/Event_Handler.h>
#include <ace/Svc_Handler.h>
#include <ace/SOCK_Connector.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Stream.h>
#include <ace/Reactor.h>
#include <ace/Connector.h>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <csignal>
#include "../interceptor.h"

class PeriodSender: public ACE_Svc_Handler<ACE_SOCK_Stream, ACE_NULL_SYNCH>{
public:
    PeriodSender(int argc, char** argv, std::shared_ptr<EndPoint> endpoint_);
    PeriodSender(){
        std::cout << "PeriodSender " << this << " constructed" << std::endl << std::flush;
    }
    ~PeriodSender(){
        std::cout << "PeriodSender " << this << " destructed" << std::endl << std::flush;
    }
    virtual int handle_timeout(const ACE_Time_Value& value, const void *pvoid);
    virtual int open(void *);
    virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask){
        delete this;
        return 0;
    }
private:
    std::shared_ptr<EndPoint> endpoint;
    std::string message;
    ACE_Time_Value interval;
    int counter;
};

class ClientConnector: public ACE_Connector<PeriodSender, ACE_SOCK_Connector>{
public:
    ClientConnector(){
        std::cout << "ClientConnector " << this << " constructed" << std::endl << std::flush;
    }
    ~ClientConnector(){
        std::cout << "ClientConnector " << this << " deconstructed" << std::endl << std::flush;
    }
    virtual int connect(PeriodSender *&sh,
                        const ACE_INET_Addr& remote_addr,
                        const ACE_Synch_Options& synch_options = ACE_Synch_Options::defaults,
                        const ACE_INET_Addr& local_addr = (ACE_INET_Addr&) ACE_INET_Addr::sap_any,
                        int reuse_addr = 0,
                        int flags = O_RDWR,
                        int perms = 0);
};

#endif
