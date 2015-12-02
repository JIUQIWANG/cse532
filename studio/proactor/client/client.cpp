//
// Created by yanhang on 11/18/15.
//

#include "client.h"
#include "../socket_handler.h"
using namespace std;

PeriodSender::PeriodSender(const std::string message_str_): message_str(message_str_),
        interval(3, 0), counter(0){
    //message.init(buffer, strlen(buffer));
    message.init(BUFSIZ);
    message.copy(message_str.c_str(), message_str.size());

    cout << "PeriodSender " << this << " constructed" << endl << std::flush;
}

void PeriodSender::open(ACE_HANDLE new_handle, ACE_Message_Block& message_bloc) {
    cout << "PeriodSender::open() called" << endl << std::flush;
    writer.open(*this, new_handle,0 , proactor());
    if(writer.write(message, BUFSIZ) < 0){
        cerr << "PeriodSender::handle_time_out(): can not write" << endl;
    }
}

void PeriodSender::handle_time_out(const ACE_Time_Value &value, const void *pVoid) {
    printf("sending data...");
    if(writer.write(message, BUFSIZ) < 0){
        cerr << "PeriodSender::handle_time_out(): can not write" << endl;
    }
}

PeriodSender* ClientConnector::make_handler() {
	cout << "Making handler" << endl << std::flush;
    PeriodSender* sender = 0;
    ACE_NEW_NORETURN(sender, PeriodSender(message_str));
    if(!sender) return 0;
    return sender;
}

void PeriodSender::handle_write_stream(const ACE_Asynch_Write_Stream::Result& result) {
    cout << "handle_write_stream called" << endl << flush;
    ACE_Message_Block &mblk = result.message_block();
    if(!result.success()){
        mblk.rd_ptr(mblk.base());
        return;
    }
	
	ACE_Proactor* curproactor = proactor() ? proactor() : ACE_Proactor::instance();
    bool can_write = handle() == result.handle();
    if(mblk.length() == 0){
        mblk.rd_ptr(mblk.base());
        if(can_write){
            curproactor->schedule_timer(*this, 0, interval);
		}
    }else if(can_write){
        if(writer.write(message, message.length()) < 0){
            cerr << "PeriodSender::handle_write_stream(): can not write" << endl;
        }
    } else{
        mblk.rd_ptr(mblk.base());
    }
}
