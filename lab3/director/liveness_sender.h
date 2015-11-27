//
// Created by Yan Hang on 11/26/15.
//

#ifndef LAB3_LIVENESS_SENDER_H
#define LAB3_LIVENESS_SENDER_H

#include <ace/ACE.h>
#include <ace/Event_Handler.h>
#include <ace/Time_Value.h>
#include <ace/INET_Addr.h>
#include <ace/Reactor.h>
#include <memory>
#include "director.h"

class Liveness_sender: public ACE_Event_Handler {
public:
	Liveness_sender(const ACE_INET_Addr& addr_, const std::shared_ptr<Director>& director_, const unsigned short local_port_):
			remote_addr(addr_), director(director_), local_port(local_port_){}
	virtual int handle_timeout(const ACE_Time_Value& value, const void *pvoid);
private:
	ACE_INET_Addr remote_addr;
	const std::shared_ptr<Director>& director;
	const unsigned short local_port;
};


#endif //LAB3_LIVENESS_SENDER_H
