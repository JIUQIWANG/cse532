#include "server_acceptor.h"

using namespace std;

int SocketHandler::handle_input(ACE_HANDLE h){
    char buffer[BUFSIZ] = {};
    if(peer().recv(buffer, BUFSIZ) <= 0){
        return -1;
    }
    string res;
    for(int i=0; i<strlen(buffer); i++){
        if(buffer[i] == '\n')
            break;
        res += buffer[i];
    }
    cout << endpoint->decode(res) << endl << std::flush;
    return 0;
}

int Server_Acceptor::make_svc_handler(SocketHandler *&sh){
    ACE_NEW_RETURN(sh, SocketHandler(endpoint), -1);
    sh->reactor(reactor());
    return 0;
}