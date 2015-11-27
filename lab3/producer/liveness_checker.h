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
#include <unordered_set>
#include "playlist.h"

typedef std::unordered_set<std::string> unique_set;

class LivenessChecker: public ACE_Event_Handler {
public:
	LivenessChecker(const std::shared_ptr<PlayList>& playlist_,
	                const std::shared_ptr<unique_set>& unique_addr_):
			playlist(playlist_), unique_addr(unique_addr_){}
	virtual int handle_timeout(const ACE_Time_Value& value, const void *pvoid);
private:
	std::shared_ptr<PlayList> playlist;
	std::shared_ptr<unique_set> unique_addr;
};

#endif //LAB3_LIVENESS_RECEIVER_H
