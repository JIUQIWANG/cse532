//
// Created by Yan Hang on 11/25/15.
//


#include "sender.h"
using namespace std;

int OutputHandler::sendMessage(const string& str) {
	ACE_SOCK_Stream &stream = this->peer();
	cout << str << ' '<< str.size() << endl;
	if (stream.send_n(str.c_str(), (int) str.size()) < 0)
		return -1;
	stream.close();
	return 0;
}

Connector Sender::connector = Connector();

int Sender::sendMessage(const std::string& str, const ACE_INET_Addr& remote_addr){
	OutputHandler* h;
	ACE_NEW_RETURN(h, OutputHandler(), -1);
	connector.connect(h, remote_addr);
	if(h->sendMessage(str) < 0) {
		connector.close();
		return -1;
	}
	connector.close();
	return 0;
}