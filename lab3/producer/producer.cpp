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
}

int Producer::handle_input(ACE_HANDLE h){
    char buf[BUFSIZ];
    string str;
    if(h == ACE_STDIN){
        ssize_t result = ACE_OS::read(h, buf, BUFSIZ);
        if(result <= 0)
            return -1;
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
    const size_t min_len = 2;
    if(str_split.size() != min_len){
        cerr << "Your command should contain at least two words" << endl;
        return -1;
    }
    ACE_INET_Addr addr(port, ACE_LOCALHOST);
    OutputHandler* h;
    ACE_NEW_RETURN(h, OutputHandler(), -1);
    connector.connect(h, addr);
    if(h->sendMessage(str) < 0)
        return -1;
    connector.close();
    return 0;
}

void Producer::close(){
}
