//
// Created by Yan Hang on 11/26/15.
//

#include "liveness_checker.h"

using namespace std;
int LivenessChecker::handle_timeout(const ACE_Time_Value& value, const void *pvoid){
	unique_set addr_to_remove;
	const list<PlayItem>& list_data = playlist->getList();
	for(const auto& v: list_data){
		if(v.is_connected)
			continue;
		char buffer[1024];
		v.addr.addr_to_string(buffer, 1024);
		addr_to_remove.insert(string(buffer));
	}

	//remove unconnected addr
	for(const auto& v: addr_to_remove){
		unique_addr->erase(v);
		ACE_INET_Addr addr;
		addr.string_to_addr(v.c_str());
		playlist->removeAddr(addr);
	}
	return 0;
}