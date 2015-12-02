//
// Created by yanhang on 11/18/15.
//

#include "client.h"
#include "../socket_handler.h"
using namespace std;

PeriodSender::PeriodSender(int argc, char** argv):
	interval(3, 0), counter(0){
		for(int i=1; i < argc; i++){
			message.append(argv[i]);
			message.append(" ");
		}
		cout << "PeriodSender " << this << " constructed" << endl << std::flush;
}

int PeriodSender::open(void *) {
	if(reactor() &&
		reactor()->schedule_timer(this, 0, interval, interval) == -1)
		return -1;
	else return 0;
}

int PeriodSender::handle_timeout(const ACE_Time_Value &value, const void *pVoid) {
	printf("sending data...");
	ACE_SOCK_Stream& stream = peer();

	if (stream.send_n(message.c_str(), message.size()) < 0) {
		cerr << endl << "Can not send message" << endl;
		return -1;
	}

	printf("data sent\n");
	return 0;
}

int ClientConnector::connect(PeriodSender *&sh, const ACE_INET_Addr &remote_addr,
							 const ACE_Synch_Options &synch_options, const ACE_INET_Addr &local_addr, int reuse_addr,
							 int flags, int perms) {
								 if(sh == 0){
									 cerr << "Service handler must be constructed first" << endl;
									 return -1;
								 }
								 unique_ptr<PeriodSender> guard_sender(sh);
								 if(connect_svc_handler(sh, remote_addr, 0, local_addr, reuse_addr, flags, perms) < 0)
									 return -1;
								 if(activate_svc_handler(sh) < 0){
									 return -1;
								 }
								 guard_sender.release();
								 return 0;
}
