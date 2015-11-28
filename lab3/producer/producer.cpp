#include "producer.h"

using namespace std;

//check connection of all directors in every 3 seconds
const ACE_Time_Value Producer::check_interval(3, 0);

Producer::Producer(const unsigned short port_, ACE_Reactor* reactor_): port(port_), reactor(reactor_), playlist(new PlayList()), unique_addr(new unique_set()), acceptor(nullptr){
    if(ACE_Event_Handler::register_stdin_handler(this, reactor, ACE_Thread_Manager::instance()) < 0){
        throw runtime_error("Producer::Producer():Failed to register keyboard handler!");
    }
    ACE_INET_Addr local_addr(port);
    ACE_NEW_NORETURN(acceptor, ProducerAcceptor(playlist, unique_addr));
    if(!acceptor){
        throw runtime_error("Producer::Producer(): Failed to allocate acceptor");
    }
    if(acceptor->open(local_addr) < 0){
        throw runtime_error("Producer::Producer():Failed to open acceptor!");
    }
    if(reactor->register_handler(acceptor, ACE_Event_Handler::ACCEPT_MASK) < 0){
        throw runtime_error("Producer::Producer():Failed to register acceptor!");
    }

    //register the liveness checker
    LivenessChecker *checker = nullptr;
    ACE_NEW_NORETURN(checker, LivenessChecker(playlist));
    if(!checker){
        throw runtime_error("Producer::Producer(): Failed to allocate LivenessChecker");
    }
    //reactor->schedule_timer(checker, 0, check_interval, check_interval);
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
    string command;
    shared_ptr<ACE_SOCK_Stream> stream;

    //to quit the producer, the producer first send <quit> command to all connected
    if(str_split.front().compare("quit") == 0){
        command = Protocal::composeCommand(Protocal::P_QUIT, "", port);
        return 0;
    }

    if(str_split.size() != 2){
        cerr << "Format: play <id-of-script>" << endl;
        return -1;
    }

    ACE_INET_Addr remote_addr;
    if(!playlist->find(str_split.back(), stream)){
        cerr << "Invalid script id!" << endl;
        return -1;
    }

    string id_converted = playlist->convertId(str_split.back());
    if(id_converted.size() == 0){
        cerr << "Invalid script id" << endl;
        return -1;
    }

    if(str_split.front().compare("play") == 0){
        command = Protocal::composeCommand(Protocal::P_PLAY, id_converted, port);
    }else if(str_split.front().compare("stop") == 0){
        command = Protocal::composeCommand(Protocal::P_STOP, id_converted, port);
    }else{
        cerr << "Invalid command." << endl;
        return -1;
    }

    char buffer_addr[BUFSIZ];
    stream->get_remote_addr(remote_addr);
    remote_addr.addr_to_string(buffer_addr, BUFSIZ);
    cout << "Remote address: " << buffer_addr << endl;
    ssize_t res = stream->send(command.c_str(), (int)command.size());
    cout << res << " bytes sent" << endl;
    if(res < 0)
        return -1;
    return 0;
}

void Producer::close(){
}
