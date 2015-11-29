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