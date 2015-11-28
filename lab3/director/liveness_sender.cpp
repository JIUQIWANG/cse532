//
// Created by Yan Hang on 11/26/15.
//

#include "liveness_sender.h"
#include "../sender.h"
#include "../common.h"
#include <string>

using namespace std;

int Liveness_sender::handle_timeout(const ACE_Time_Value &value, const void *pvoid) {
	string command = Protocal::composeCommand(Protocal::P_LIST, director->getPlayList(), local_port);
	return 0;//Sender::sendMessage(command, remote_addr);
}
