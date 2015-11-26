#include "director_ace.h"
#include "../signal_handler.h"
#include <iostream>
#include <memory>

//define some situations for return value and variables of extra part 2
#define RETURN_SUCCESS 0
#define RETURN_SCRIPT_FILE_NOT_FOUND 1
#define RETURN_DIRECTOR_CUE_FAILED 2
#define OVERRIDE true
#define NOTOVERRIDE false

using namespace std;

int main(int argc, char** argv) {
	//judge the correctness of number of arguments.
	if (argc < 4) {
		cerr << "Usage: " << argv[0] << " <port> <ip_address> <min_threads> <script_file>+" << endl;
		return 1;
	}

	shared_ptr<Director> director;
	try {
		int minimum_value = stoi(argv[3]);
		vector<string> scripts;
		for (int pos = 4; pos < argc; pos++) {
			scripts.push_back(argv[pos]);
		}
		director = shared_ptr<Director>(new Director(minimum_value, scripts));
	}
	catch (exception e) {
		cerr << e.what() << endl;
		return RETURN_SCRIPT_FILE_NOT_FOUND;
	}

	//initialize connection
	ACE_INET_Addr addr;
	unsigned short local_port;
	string addr_str = string(argv[2]) + ':' + string(argv[1]);
	cout << "Remote address: " << addr_str << endl;
	addr.string_to_addr(addr_str.c_str());
	DirectorAcceptor acceptor(director);
	if(initializeAcceptor(acceptor, local_port) < 0){
		cerr << "Can not open acceptor!" << endl;
		return returnType::ECONNECTION;
	}

	if(sendPlayList(director, addr, local_port) < 0){
		cerr << "Can not connect to Producer" << endl;
		return returnType::ECONNECTION;
	}
	cout << "Local acceptor opened, port:" << local_port << endl;

	//start main loop
	SignalHandler* sighandler;
	ACE_NEW_RETURN(sighandler, SignalHandler(), returnType::EMEMORY);
	ACE_Reactor::instance()->register_handler(SIGINT, sighandler);

	while(true){
		if(SignalHandler::is_interrupted()) {
			cout << "closed" << endl << flush;
			closeConnection();
			break;
		}
		ACE_Reactor::instance()->handle_events();
	}

//	try {
//		string input;
//		vector<string> result;
//		while (getline(cin, input)) {
//			director->handler(input, result);
//			for (auto str : result){
//				cout << str << endl;
//			}
//			result.clear();
//		}
//	}
//	catch (exception e) {
//		cerr << e.what() << endl;
//		return RETURN_DIRECTOR_CUE_FAILED;
//	}

	return RETURN_SUCCESS;
}


