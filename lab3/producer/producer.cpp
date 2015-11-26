#include "producer.h"

using namespace std;

Producer::Producer(const unsigned short port_, ACE_Reactor* reactor_): port(port_), reactor(reactor_), playlist(new PlayList()), acceptor(playlist), connector(){
    if(ACE_Event_Handler::register_stdin_handler(this, reactor, ACE_Thread_Manager::instance()) < 0){
        throw runtime_error("Producer::Producer():Failed to register keyboard handler!");
    }
    ACE_INET_Addr local_addr(port, ACE_LOCALHOST);
    if(acceptor.open(local_addr) < 0){
        throw runtime_error("Producer::Producer():Failed to open acceptor!");
    }
    cout << "Waiting for director..." << endl;
}

int Producer::handle_input(ACE_HANDLE h){
    char buf[BUFSIZ];
    string str;
    if(h == ACE_STDIN){
        ssize_t result = ACE_OS::read(h, buf, BUFSIZ);
        if(result <= 0)
            return -1;
        if(buf[0] == '\n')
            return 0;
        for(int i=0; i<strlen(buf); i++){
            if(buf[i] == '\n')
                break;
            str += buf[i];
        }
        handleKeyboard(str);
    }
    return 0;
}

int Producer::handleKeyboard(const string& str){
    vector<string> str_split;
    string_util::split_str(str, str_split);

    if(str_split.front().compare("quit") == 0){
        return 0;
    }

    if(str_split.size() != 2){
        cerr << "Format: play <id-of-script>" << endl;
        return -1;
    }

    ACE_INET_Addr remote_addr;
    if(!playlist->find(str_split.back(), remote_addr)){
        cerr << "Invalid script id!" << endl;
        return -1;
    }

    std::string command;
    if(str_split.front().compare("play") == 0){
        command = Protocal::composeCommand(Protocal::P_PLAY, str_split.back(), port);
    }else if(str_split.front().compare("stop") == 0){
        command = Protocal::composeCommand(Protocal::P_STOP, str_split.back(), port);
    }else{
        cerr << "Invalid command." << endl;
        return -1;
    }

    OutputHandler* h;
    ACE_NEW_RETURN(h, OutputHandler(), -1);
    connector.connect(h, remote_addr);
    if(h->sendMessage(command) < 0)
        return -1;
    connector.close();
    return 0;
}

void Producer::close(){
}
