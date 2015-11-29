#include "director_agent.h"
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

	shared_ptr<DirectorAgent> agent;
	try {
		agent = shared_ptr<DirectorAgent>(new DirectorAgent(argc, argv));
	}
	catch (const runtime_error& e) {
		cerr << e.what() << endl;
		return RETURN_SCRIPT_FILE_NOT_FOUND;
	}

	//start main loop
	int openStatus = agent->open(argv);
	if(openStatus != returnType::SUCCESS)
	    return openStatus;
	return agent->run();
}

