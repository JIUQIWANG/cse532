//
// Created by Yan Hang on 11/25/15.
//


#include "outputHandler.h"
using namespace std;

int OutputHandler::sendMessage(const string& str) {
	ACE_SOCK_Stream &stream = this->peer();
	if (stream.send_n(str.c_str(), (int) str.size()) < 0)
		return -1;
	stream.close();
	return 0;
}
