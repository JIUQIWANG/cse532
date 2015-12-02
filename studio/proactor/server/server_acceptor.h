#ifndef SERVER_ACCEPTOR_H
#define SERVER_ACCEPTOR_H
#include <ace/ACE.h>
#include <memory>
#include <ace/Asynch_IO.h>
#include <ace/Asynch_Acceptor.h>
#include <ace/Message_Block.h>
#include <ace/Proactor.h>
#include <ace/Unbounded_Set.h>
#include <ace/OS.h>
#include <iostream>

class Server_Acceptor;

class SocketHandler : public ACE_Service_Handler {
public:
    SocketHandler(Server_Acceptor* acceptor_): acceptor(acceptor_){
        std::cout << "SocketHandler " << this << " constructed" << std::endl;
    }
	SocketHandler(): acceptor(0){}
    virtual ~SocketHandler();
    virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result& result);
	virtual void open(ACE_HANDLE new_handle, ACE_Message_Block& message_block);
private:
	ACE_Asynch_Read_Stream reader;
	Server_Acceptor* acceptor;
	ACE_Message_Block mblk;
};

class Server_Acceptor: public ACE_Asynch_Acceptor<SocketHandler>{
public:
    Server_Acceptor(){
        std::cout << "Server_Acceptor " << this << " constructed" << std::endl << std::flush;
    }
    ~Server_Acceptor(){
        std::cout << "Server_Acceptor " << this << " destructed" << std::endl << std::flush;
    }
	virtual SocketHandler* make_handler();
	inline void remove(SocketHandler* ih){
		client.remove(ih);
	}
    void close();
private:
	ACE_Unbounded_Set<SocketHandler*> client;
};
#endif
