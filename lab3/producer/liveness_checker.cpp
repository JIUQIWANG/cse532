//
// Created by Yan Hang on 11/26/15.
//

#include "liveness_checker.h"

using namespace std;
int LivenessChecker::handle_timeout(const ACE_Time_Value& value, const void *pvoid){
	unordered_set<ACE_INET_Addr> addr_to_remove;
	const list<PlayItem>& list_data = playlist->getList();
	for(const auto& v: list_data){
		if(v.is_connected)
			continue;
		addr_to_remove.insert(v.addr);
	}

	//remove unconnected addr
	for(const auto& v: addr_to_remove){
		playlist->removeAddr(v);
	}
	return 0;
}