#include "director_acceptor.h"
#include "../common.h"

using namespace std;

int DirectorInputHandler::handle_input(ACE_HANDLE h){
    char data[1024] = {};
    ACE_SOCK_Stream& stream = peer();
    string str;
    while(true){
        ssize_t res = stream.recv(data,1024);
        if(res <= 0)
            break;
        str.append(data);
    }
    director->parseCommand(str);
    stream.close();
    return 0;
}

int sendPlayList(const shared_ptr<Director>& director, const ACE_INET_Addr& addr, const unsigned short local_port){
    Connector connector;
    OutputHandler *h;
    ACE_NEW_RETURN(h, OutputHandler(), -1);
    if(connector.connect(h, addr) < 0)
        return -1;
    string str = Protocal::composeCommand(Protocal::P_LIST, director->getPlayList(), local_port);
    h->sendMessage(str);
    return 0;
}

int initializeAcceptor(DirectorAcceptor& acceptor, unsigned short& local_port){
    //search for an avaliable port
    const unsigned short start_port = 3000;
    const unsigned short end_port = 10000;
    for(unsigned short i=start_port; i<=end_port; i++){
        if(acceptor.open(ACE_INET_Addr(i, ACE_LOCALHOST)) >= 0){
            local_port = i;
            return 0;
        }
    }
    return -1;
}