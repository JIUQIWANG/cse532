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
#include "../common.h"

class PeriodSender: public ACE_Svc_Handler<ACE_SOCK_Stream, ACE_NULL_SYNCH>{
public:
    PeriodSender(): interval(3,0), counter(0), status("uninited"){
        Service_Logger::registerService(this, "PeriodSender");
        std::cout << "PeriodSender " << this << " constructed" << std::endl << std::flush;
    }
    ~PeriodSender(){
        Service_Logger::unregister(this);
        std::cout << "PeriodSender " << this << " destructed" << std::endl << std::flush;
    }

    virtual int init(int argc, ACE_TCHAR* argv[]){
        if(argc < 2){
            std::cerr << "Invalid argument" << std::endl;
            return -1;
        }
        for(int i=1; i<argc; i++){
            message += std::string(argv[i]);
            message += " ";
        }
        status = "inited";
        std::cout << "PeriodSender::init() called" << std::endl << std::flush;
        return 0;
    }

    virtual int info(ACE_TCHAR** strp, size_t length) const{
        std::cout << "PeriodSender " << this << " " << status << std::endl;
        return 0;
    }
    virtual int resume(){
        ACE_Reactor::instance()->schedule_timer(this, 0, interval, interval);
        status = "running";
        std::cout << "PeriodSender::resume() called" << std::endl << std::flush;
        return 0;
    }
    virtual int suspend(){
        ACE_Reactor::instance()->cancel_timer(this);
        status = "paused";
        std::cout << "PeriodSender::suspend() called" << std::endl << std::flush;
        return 0;
    }
    virtual int fini(){
        reactor()->cancel_timer(this);
        delete this;
        return 0;
    }

    virtual int handle_timeout(const ACE_Time_Value& value, const void *pvoid);
    virtual int open(void *);
    virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask){
        delete this;
        return 0;
    }
private:
    std::string message;
    ACE_Time_Value interval;
    std::string status;
    int counter;
};

class ClientConnector: public ACE_Connector<PeriodSender, ACE_SOCK_Connector>{
public:
    ClientConnector():port(2000), status("uninited"){
        Service_Logger::registerService(this, "ClientConnector");
        std::cout << "ClientConnector " << this << " constructed" << std::endl << std::flush;
    }
    ~ClientConnector(){
        Service_Logger::unregister(this);
        std::cout << "ClientConnector " << this << " deconstructed" << std::endl << std::flush;
    }
    virtual int init(int argc, ACE_TCHAR* argv[]){
        ACE_INET_Addr addr(port, ACE_LOCALHOST);
        PeriodSender* sender;
        ACE_NEW_RETURN(sender, PeriodSender(), -1);

        if(connect(sender, addr) < 0){
            std::cerr << "Can not connect to server" << std::endl;
            return -1;
        }
        return 0;
    }

    virtual int info(ACE_TCHAR** strp, size_t length) const{
        std::cout << "ClientConnector " << this << " " << status << std::endl;
        return 0;
    }
    virtual int resume(){
        std::cout << "PeriodSender::resume() called, but nothing to do" << std::endl << std::flush;
        return 0;
    }
    virtual int suspend(){
        std::cout << "PeriodSender::suspend() called, but nothing to do" << std::endl << std::flush;
        return 0;
    }
    virtual int fini(){
        delete this;
        return 0;
    }

    virtual int connect(PeriodSender *&sh,
                        const ACE_INET_Addr& remote_addr,
                        const ACE_Synch_Options& synch_options = ACE_Synch_Options::defaults,
                        const ACE_INET_Addr& local_addr = (ACE_INET_Addr&) ACE_INET_Addr::sap_any,
                        int reuse_addr = 0,
                        int flags = O_RDWR,
                        int perms = 0);
private:
    PeriodSender* sender;
    const unsigned short port;
    std::string status;
};

#endif
