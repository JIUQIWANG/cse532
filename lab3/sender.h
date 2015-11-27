//
// Created by Yan Hang on 11/25/15.
//

#ifndef OUTPUTHANDLER_H
#define OUTPUTHANDLER_H
#include <ace/ACE.h>
#include <ace/Svc_Handler.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Connector.h>
#include <ace/Connector.h>
#include <string>
class OutputHandler: public ACE_Svc_Handler<ACE_SOCK_Stream, ACE_NULL_SYNCH> {
public:
	int sendMessage(const std::string& str);
};

typedef ACE_Connector<OutputHandler, ACE_SOCK_Connector> Connector;

class Sender{
public:
	static int sendMessage(const std::string& str, const ACE_INET_Addr& addr);
private:
	static Connector connector;
};
#endif //LAB3_OUTPUTHANDLER_H
