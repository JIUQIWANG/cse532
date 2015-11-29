//
// Created by Yan Hang on 11/26/15.
//

#include "liveness_checker.h"

using namespace std;
int LivenessChecker::handle_timeout(const ACE_Time_Value& value, const void *pvoid){
	list<ItemSet> data = playlist->getList();
	string checkmsg = Protocal::composeCommand(Protocal::P_CHECK, string(""), 0);
	for(auto& v: data){
		ACE_INET_Addr remote_addr;
		v.stream->get_remote_addr(remote_addr);
		char addr_buffer[BUFSIZ];
		remote_addr.addr_to_string(addr_buffer, BUFSIZ);
		if(Sender::sendMessage(checkmsg, *v.stream) != Sender::SUCCESS){
			cerr << "Director " << addr_buffer << " offline, remove scripts from it" << endl;
			playlist->removeAddr(remote_addr);
		}
	}
	return 0;
}