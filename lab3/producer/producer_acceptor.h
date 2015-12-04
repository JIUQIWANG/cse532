#ifndef PRODUCER_ACE_H
#define PRODUCER_ACE_H
#include <ace/ACE.h>
#include <ace/Acceptor.h>
#include <ace/Event_Handler.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Connector.h>
#include <ace/SOCK_Stream.h>
#include <ace/Svc_Handler.h>
#include <ace/INET_Addr.h>
#include <ace/OS.h>
#include <ace/Reactor.h>
#include <memory>
#include <iostream>
#include <map>
#include "liveness_checker.h"
#include "../common.h"
#include "../signal_handler.h"
#include "playlist.h"

class ProducerInputHandler : public ACE_Svc_Handler<ACE_SOCK_Stream, ACE_NULL_SYNCH> {
public:
    inline void setPlayList(const std::shared_ptr<PlayList> playlist_) {
        playlist = playlist_;
    }
    int parseCommand(const std::string& str);
    virtual int handle_input(ACE_HANDLE=ACE_INVALID_HANDLE);
    virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask){
        delete this;
        return SUCCESS_RETURN;
    }
private:
    inline void updateScreen(){
        CLEAN_SCREEN;
        cout << "Current list:" << endl;
        playlist->printList();
        Protocal::printInstruction();
    }

    std::shared_ptr<PlayList> playlist;
	static const ACE_Time_Value timeout;
};

class ProducerAcceptor: public ACE_Acceptor<ProducerInputHandler, ACE_SOCK_Acceptor>{
public:
    ProducerAcceptor(const std::shared_ptr<PlayList> playlist_):
            playlist(playlist_){}
    virtual int make_svc_handler(ProducerInputHandler *&sh){
        ProducerInputHandler *h;
        ACE_NEW_RETURN(h, ProducerInputHandler(), -1);
        sh = h;
        sh->setPlayList(playlist);
        sh->reactor(reactor());
        return SUCCESS_RETURN;
    }
    virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask){
        delete this;
        return SUCCESS_RETURN;
    }
private:

    const std::shared_ptr<PlayList> playlist;
};
#endif
