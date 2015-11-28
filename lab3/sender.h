#ifndef SENDER_H
#define SENDER_H

#include <ace/ACE.h>
#include <ace/SOCK_Stream.h>
#include <ace/Time_Value.h>
#include <string>
#include <iostream>

class Sender{
public:
	enum sendStatus{SUCCESS, E_SEND, E_RESPONSE};
	static int sendMessage(const std::string& str, const ACE_SOCK_Stream& stream);
private:
	static const ACE_Time_Value timeout;
	static char response[BUFSIZ];
};

#endif
