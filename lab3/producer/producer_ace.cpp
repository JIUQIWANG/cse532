#include "producer_ace.h"

using namespace std;

int ProducerInputHandler::handle_input(ACE_HANDLE h){
    char data[1024];
    ACE_SOCK_Stream& stream = peer();
    string str;
    while(true){
        ssize_t res = stream.recv(data,1024);
        if(res <= 0)
            break;
        str.append(data);
    }
    stream.close();
    return parseCommand(str);
}

int ProducerInputHandler::parseCommand(const std::string &str) {
    vector<string> play_title;
    Protocal::protocalType type;
    unsigned short remote_port;
    ACE_INET_Addr remote_addr;
    peer().get_remote_addr(remote_addr);

    if(Protocal::parseCommand(str, type, play_title, remote_port) == -1)
        return -1;

    remote_addr.set_port_number(remote_port);
    for(const auto& v: play_title){
        cout << v << endl;
        PlayItem item(v, remote_addr);
        playlist->push_back(item);
    }
    playlist->printList();
    Protocal::printInstruction();

    return 0;
}