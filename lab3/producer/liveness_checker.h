//
// Created by Yan Hang on 11/26/15.
//

#ifndef LAB3_LIVENESS_RECEIVER_H
#define LAB3_LIVENESS_RECEIVER_H
#include <ace/ACE.h>
#include <ace/Event_Handler.h>
#include <ace/INET_Addr.h>
#include <ace/Reactor.h>
#include <memory>
#include "playlist.h"
#include "../common.h"
#include "../sender.h"

class LivenessChecker: public ACE_Event_Handler {
public:
	LivenessChecker(const std::shared_ptr<PlayList>& playlist_):
			playlist(playlist_){}
	virtual int handle_timeout(const ACE_Time_Value& value, const void *pvoid);
	virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask){
		delete this;
		return SUCCESS_RETURN;
	}
private:
	std::shared_ptr<PlayList> playlist;
};

#endif //LAB3_LIVENESS_RECEIVER_H
