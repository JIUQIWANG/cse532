//
// Created by Yan Hang on 11/25/15.
//


#include "sender.h"
using namespace std;

const ACE_Time_Value Sender::timeout = ACE_Time_Value(5);
char Sender::response[BUFSIZ] = {};

int Sender::sendMessage(const std::string& str, const ACE_SOCK_Stream& stream){
	ssize_t res = stream.send_n(str.c_str(), (int)str.size(), &timeout);
	if(res != (ssize_t)str.size()){
		cerr << "Sender::sendMessage(): sending timeout!"<<endl;
		return E_SEND;
	}
	res = stream.recv(response, 1, &timeout);
	if(res <= 0){
		cerr << "Sender::sendMessage(): response timeout!" << endl;
		return E_RESPONSE;
	}
	return SUCCESS;
}