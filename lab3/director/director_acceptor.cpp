#include "director_acceptor.h"
#include "../common.h"

using namespace std;

const ACE_Time_Value DirectorInputHandler::timeout = ACE_Time_Value(5);

int DirectorInputHandler::handle_input(ACE_HANDLE h){
    char data[BUFSIZ] = {};
    ACE_SOCK_Stream& stream = peer();
    string str;
    ssize_t res = stream.recv(data,BUFSIZ);
    if(res <= 0)
        return -1;
	res = stream.send_n("K", 1, &timeout);
	if(res <= 0)
		return -1;
    str.append(data);
    director->parseCommand(str);
    return 0;
}

void DirectorInputHandler::parseCommand(const string& str){
	if(string_util::is_empty(command)){
		res = "Empty Command";
		return res;
	}
	Protocal::protocalType type;
	vector<string> arguments;
	unsigned short remote_port;
	Protocal::parseCommand(command, type, arguments, remote_port);
	//Command is quit
	if(type == Protocal::P_QUIT) {
		cout << "quit" << endl;
		SignalHandler::interrupt();
		play_queue.push_back(finish_token);
		res = "Quit!";
		return res;
	}

	int play_id = std::stoi(arguments.back());
	//The operated script ID is invalid
	if(play_id >= (int)scripts.size() || play_id < 0){
		res = "Invalid Script ID";
		return res;
	}

	//Command is play
	string command;
	if (type == Protocal::P_PLAY){
		unique_lock<mutex> guard(mt);
		play_queue.push_back(play_id);
		guard.unlock();
		res = "Add " + to_string(play_id) + " in the play queue";
		cv.notify_all();
	}
	//Command is stop
	else if (type == Protocal::P_STOP){
		res = stop(play_id);
	}
}