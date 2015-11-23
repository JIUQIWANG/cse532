#ifndef PRODUCER_H
#define PRODUCER_H
#include <memory>
#include <string>
#include "producer_ace.h"

class Producer{
public:
    enum commandType{
	PLAY,
	STOP,
	EXIT
    };
    Producer(const unsigned short port_);

    //handle keyboard interaction
    int handleKeyboard(const std::string& str);
    
    //when shutting down, notify all connected directors
    void close();
private:
    const unsigned short port;
    //use shared_ptr to avoid memory leak

    std::shared_ptr<PlayList> playlist;
    std::shared_ptr<ProducerAcceptor > acceptor;
//    std::shared_ptr<ACE_Connector<ProducerOutputHandler, ACE_SOCK_Connector> > connector;

};
#endif
