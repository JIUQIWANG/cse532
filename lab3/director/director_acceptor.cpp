#include "director_acceptor.h"
#include "../common.h"

using namespace std;

int DirectorInputHandler::handle_input(ACE_HANDLE h){
    char data[BUFSIZ] = {};
    ACE_SOCK_Stream& stream = peer();
    string str;
    ssize_t res = stream.recv(data,BUFSIZ);
    if(res <= 0)
        return -1;
    str.append(data);
    cout << "command received: " << str << endl;
    director->parseCommand(str);
    return 0;
}

int sendPlayList(const std::shared_ptr<Director>& director, const ACE_SOCK_Stream& stream, const unsigned short local_port){
    string str = Protocal::composeCommand(Protocal::P_LIST, director->getPlayList(), local_port);
    if(stream.send(str.c_str(), str.size()) < 0)
        return -1;
    return 0;
}

int initializeAcceptor(DirectorAcceptor& acceptor, unsigned short& local_port){
    //search for an avaliable port
    const unsigned short start_port = 3000;
    const unsigned short end_port = 10000;
    for(unsigned short i=start_port; i<=end_port; i++){
        if(acceptor.open(ACE_INET_Addr(i)) >= 0){
            local_port = i;
            return 0;
        }
    }
    return -1;
}
