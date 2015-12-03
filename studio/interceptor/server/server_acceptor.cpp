#include "server_acceptor.h"
#include "../socket_handler.h"

using namespace std;

int Server_Acceptor::handle_input(ACE_HANDLE h){
    ACE_Acceptor<SocketHandler, ACE_SOCK_Acceptor>::handle_input(h);
    return 0;
}