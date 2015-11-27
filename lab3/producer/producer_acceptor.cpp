#include "producer_acceptor.h"

using namespace std;

int ProducerInputHandler::handle_input(ACE_HANDLE h){
    char data[1024] = {};
    ACE_SOCK_Stream& stream = peer();
    string str;
    while(true){
        ssize_t res = stream.recv(data,1024);
        if(res <= 0)
            break;
        str.append(data);
    }
    cout << str << endl;
    stream.close();
    return parseCommand(str);
}

int ProducerInputHandler::parseCommand(const std::string &str) {
    vector<string> play_title;
    Protocal::protocalType type;
    unsigned short remote_port;
    ACE_INET_Addr remote_addr;
    peer().get_remote_addr(remote_addr);

    if (Protocal::parseCommand(str, type, play_title, remote_port) == -1)
        return -1;

    remote_addr.set_port_number(remote_port);
    char addr_buffer[100] = {};
    remote_addr.addr_to_string(addr_buffer, 100);

    if (type == Protocal::P_LIST) {
        if(unique_addr->find(string(addr_buffer)) != unique_addr->end()) {
            playlist->maintainConnection(remote_addr);
            return 0;
        }
        unique_addr->insert(string(addr_buffer));
        int counter = 0;
        for (const auto &v: play_title) {
            PlayItem item(v, counter++, remote_addr);
            playlist->push_back(item);
        }
        playlist->printList();
        Protocal::printInstruction();
    }else if(type == Protocal::P_QUIT){
        playlist->removeAddr(remote_addr);
        unique_addr->erase(string(addr_buffer));
        if(playlist->is_empty())
            SignalHandler::interrupt();
        playlist->printList();
    }

    return 0;
}