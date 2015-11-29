#include "producer_acceptor.h"

using namespace std;

const ACE_Time_Value ProducerInputHandler::timeout = ACE_Time_Value(5);

int ProducerInputHandler::handle_input(ACE_HANDLE h){
    char data[BUFSIZ] = {};
    ACE_SOCK_Stream& stream = peer();
    string str;
    ssize_t res = stream.recv(data,sizeof(data));
    if(res <= 0)
        return -1;
	res = stream.send_n("K", 1, &timeout);
	if(res <= 0)
		return -1;
    str.append(data);
    return parseCommand(str);
}

int ProducerInputHandler::parseCommand(const std::string &str) {
    if(str.empty()) {
        cout << "empty" << endl;
        return -1;
    }
    vector<string> play_title;
    Protocal::protocalType type;
    unsigned short remote_port;

    if (Protocal::parseCommand(str, type, play_title, remote_port) == -1)
        return -1;

    char addr_buffer[BUFSIZ] = {};
    ACE_INET_Addr remote_addr;
    peer().get_remote_addr(remote_addr);
    remote_addr.set_port_number(remote_port);
    remote_addr.addr_to_string(addr_buffer, BUFSIZ);

    if (type == Protocal::P_LIST) {
        if(unique_addr->find(string(addr_buffer)) != unique_addr->end()) {
            playlist->maintainConnection(remote_addr);
            return 0;
        }
        unique_addr->insert(string(addr_buffer));
        int counter = 0;
        shared_ptr<ACE_SOCK_Stream> director_stream(new ACE_SOCK_Stream());
        ACE_SOCK_Connector connector;
        connector.connect(*director_stream, remote_addr);
        for (const auto &v: play_title) {
            PlayItem item(v, counter++, director_stream);
            playlist->push_back(item);
        }
        playlist->printList();
        Protocal::printInstruction();
	}else if(type == Protocal::P_PLAYING){

    }else if(type == Protocal::P_QUIT){
        playlist->removeAddr(remote_addr);
        unique_addr->erase(string(addr_buffer));
        if(playlist->is_empty())
            SignalHandler::interrupt();
        playlist->printList();
    }

    return 0;
}