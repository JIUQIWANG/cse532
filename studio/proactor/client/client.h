#ifndef CLIENT_H
#define CLIENT_H

#include <ace/ACE.h>
#include <ace/Proactor.h>
#include <ace/Asynch_IO.h>
#include <ace/Asynch_Connector.h>
#include <ace/Message_Block.h>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <csignal>

class PeriodSender: public ACE_Service_Handler{
public:
    PeriodSender(const std::string message_str_);
    PeriodSender(): interval(3,0), counter(0){
        std::cout << "PeriodSender " << this << " constructed" << std::endl << std::flush;
    }
    ~PeriodSender(){
        std::cout << "PeriodSender " << this << " destructed" << std::endl << std::flush;
    }
    virtual void handle_time_out(const ACE_Time_Value& value, const void *pvoid);
    virtual void handle_write_stream(const ACE_Asynch_Write_Stream::Result& result);
    virtual void open(ACE_HANDLE new_handle, ACE_Message_Block& message_block);

private:
    ACE_Message_Block message;
    std::string message_str;
    ACE_Asynch_Write_Stream writer;
    ACE_Time_Value interval;
    int counter;
};

class ClientConnector: public ACE_Asynch_Connector<PeriodSender>{
public:
    ClientConnector(int argc, char** argv){
        open();
        for(int i=1; i<argc; i++){
            message_str.append(argv[i]);
            message_str.append(" ");
        }
	std:: cout << "PeriodSender " << this << " constructed" << std::endl << std::flush;

        std::cout << "ClientConnector " << this << " constructed" << std::endl << std::flush;
    }
    ~ClientConnector(){
        std::cout << "ClientConnector " << this << " deconstructed" << std::endl << std::flush;
    }
    virtual PeriodSender* make_handler();
private:
    std::string message_str;
};

#endif
