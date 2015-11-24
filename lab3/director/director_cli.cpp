// lab2.cpp : Defines the entry point for the console application.
//
#include "director.h"
#include "string_util.h"
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
	director = shared_ptr<Director>(new Director(argv[1], argv[2], minimum_value, scripts));
    }
    catch (exception e) {
	cerr << e.what() << endl;
	return RETURN_SCRIPT_FILE_NOT_FOUND;
    }
    try {
	string input;
	vector<string> result;
	while (getline(cin, input)) {
	    director->handler(input, result);
	    for (auto str : result){
		cout << str << endl;
	    }
	    result.clear();
	}
    }
    catch (exception e) {
	cerr << e.what() << endl;
	return RETURN_DIRECTOR_CUE_FAILED;
    }

    return RETURN_SUCCESS;
}


