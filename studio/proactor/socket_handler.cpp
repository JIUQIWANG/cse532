/*
 * ACE reactor demonstration
 *
 * $Id: ReadHandler.cpp 97246 2013-08-07 07:10:20Z johnnyw $
 * Date: 26-Jan-2006
 */

#include "socket_handler.h"

#include <ace/Time_Value.h>

using namespace std;

int SocketHandler::handle_input(ACE_HANDLE h){
    cout << "Data received" << endl << std::flush;
    char buffer[BUFSIZ];
    if(peer().recv(buffer, BUFSIZ) <= 0){
        return -1;
    }
    cout << buffer << endl;
    return 0;
}
