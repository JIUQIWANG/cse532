#ifndef DIRECTOR_ACE_H
#define DIRECTOR_ACE_H

#include <ace/ACE.h>
#include <ace/Acceptor.h>
#include <ace/Connector.h>
#include <ace/Svc_Handler.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Connector.h>
#include <memory>
#include <string>
#include "director.h"
#include "../sender.h"

class DirectorInputHandler: public ACE_Svc_Handler<ACE_SOCK_Stream, ACE_NULL_SYNCH>{
public:
    DirectorInputHandler(){}
    DirectorInputHandler(const std::shared_ptr<Director> director_): director(director_){}

    virtual int handle_input(ACE_HANDLE=ACE_INVALID_HANDLE);
private:
    std::shared_ptr<Director> director;
};

class DirectorAcceptor: public ACE_Acceptor<DirectorInputHandler, ACE_SOCK_Acceptor>{
public:
    DirectorAcceptor(const std::shared_ptr<Director> director_): director(director_){}
    virtual int make_svc_handler(DirectorInputHandler *&sh){
        printf("Director: connection received!\n");
        DirectorInputHandler *h;
        ACE_NEW_RETURN(h, DirectorInputHandler(director), -1);
        sh = h;
        sh->reactor(reactor());
        return 0;
    }
private:
    std::shared_ptr<Director> director;
};

int sendPlayList(const std::shared_ptr<Director>& director, const ACE_SOCK_Stream& stream, const unsigned short local_port);

int initializeAcceptor(DirectorAcceptor& acceptor, unsigned short& local_port);
#endif
