#ifndef PRODUCER_ACE_H
#define PRODUCER_ACE_H
#include <ace/ACE.h>
#include <ace/Acceptor.h>
#include <ace/Connector.h>
#include <ace/Event_Handler.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Connector.h>
#include <ace/SOCK_Stream.h>
#include <ace/Svc_Handler.h>
#include <ace/Thread_Manager.h>
#include <ace/OS.h>
#include <ace/Reactor.h>
#include <memory>
#include "../common.h"
#include "../outputHandler.h"
#include "playlist.h"

class ProducerInputHandler : public ACE_Svc_Handler<ACE_SOCK_Stream, ACE_NULL_SYNCH>{
public:
    inline void setPlayList (const std::shared_ptr<PlayList> playlist_){
        playlist = playlist_;
    }

    virtual int handle_input(ACE_HANDLE=ACE_INVALID_HANDLE);
    virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask){
        printf("ProducerInputHandler::handle_close() called!\n");
        return 0;
    }

private:
    std::shared_ptr<PlayList> playlist;
};

class ProducerAcceptor: public ACE_Acceptor<ProducerInputHandler, ACE_SOCK_Acceptor>{
public:
    ProducerAcceptor(const std::shared_ptr<PlayList> playlist_): playlist(playlist_){}
    virtual int make_svc_handler(ProducerInputHandler *&sh){
        sh = new ProducerInputHandler();
        sh->setPlayList(playlist);
        return 0;
    }
private:
    const std::shared_ptr<PlayList> playlist;
};
#endif
