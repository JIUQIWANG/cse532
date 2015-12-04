#include "producer_acceptor.h"

using namespace std;

const ACE_Time_Value ProducerInputHandler::timeout = ACE_Time_Value(5);

int ProducerInputHandler::handle_input(ACE_HANDLE h){
    char data[BUFSIZ] = {};
    ACE_SOCK_Stream& stream = peer();
    string str;
    ssize_t res = stream.recv(data,sizeof(data));
    if(res <= 0)
        return ERROR_RETURN;
    const char response = 'K';
	res = stream.send_n(&response, 1, &timeout);
	if(res <= 0)
		return ERROR_RETURN;
    str.append(data);
    return parseCommand(str);
}

int ProducerInputHandler::parseCommand(const std::string &str) { ;
    if(str.empty()) {
        cout << "empty" << endl;
        return ERROR_RETURN;
    }
    vector<string> play_title;
    Protocal::protocalType type;
    unsigned short remote_port;

    if (Protocal::parseCommand(str, type, play_title, remote_port) == -1)
        return ERROR_RETURN;

    char addr_buffer[BUFSIZ] = {};
    ACE_INET_Addr remote_addr;
    peer().get_remote_addr(remote_addr);
    remote_addr.set_port_number(remote_port);
    remote_addr.addr_to_string(addr_buffer, BUFSIZ);

    if (type == Protocal::P_LIST) {
        int counter = 0;
        shared_ptr<ACE_SOCK_Stream> director_stream(new ACE_SOCK_Stream());
        ACE_SOCK_Connector connector;
        connector.connect(*director_stream, remote_addr);
        ItemSet itemset(director_stream);
        for (const auto &v: play_title){
            PlayItem item(v, counter++);
            itemset.item.push_back(item);
		}
        playlist->push_back(itemset);
	}else if(type == Protocal::P_PLAYING) {
        playlist->occupy(remote_addr);
    }else if(type == Protocal::P_FINISH){
        playlist->release(remote_addr);
    }else if(type == Protocal::P_QUIT){
        playlist->removeAddr(remote_addr);
        updateScreen();
        cout << "Director " << addr_buffer << " successfully exited" << endl;
        if(playlist->is_empty() && playlist->is_cleaning())
            QuitFlags::interrupt();
        return SUCCESS_RETURN;
    }
    updateScreen();

    return SUCCESS_RETURN;
}