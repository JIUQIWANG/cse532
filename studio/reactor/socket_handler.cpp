/*
 * ACE reactor demonstration
 *
 * $Id: ReadHandler.cpp 97246 2013-08-07 07:10:20Z johnnyw $
 * Date: 26-Jan-2006
 */

#include "socket_handler.h"

#include <ace/streams.h>
#include <ace/Time_Value.h>

using namespace std;

int SocketHandler::handle_input(ACE_HANDLE h){
    while(true){
	if(mStream.recv(&mData, 1) < 0)
	    break;
	cout << mData;
    }
    return 0;
}
