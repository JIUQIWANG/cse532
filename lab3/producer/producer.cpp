#include "producer.h"

using namespace std;

//check connection of all directors in every 3 seconds
const ACE_Time_Value Producer::check_interval(3, 0);

int QuitTimer::handle_timeout(const ACE_Time_Value &current_time, const void *act){
    cout << "No response from the following scripts:" << endl;
    playlist->printList();
    cout << "Force quit" << endl;
    SignalHandler::interrupt();
    return -1;
}

Producer::Producer(const unsigned short port_, ACE_Reactor* reactor_): port(port_), reactor(reactor_), playlist(new PlayList()), acceptor(nullptr){
    if(ACE_Event_Handler::register_stdin_handler(this, reactor, ACE_Thread_Manager::instance()) < 0){
        throw runtime_error("Producer::Producer():Failed to register keyboard handler!");
    }
    ACE_INET_Addr local_addr(port);
    ACE_NEW_NORETURN(acceptor, ProducerAcceptor(playlist));
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
    if(checker == nullptr){
        throw runtime_error("Producer::Producer(): Failed to allocate LivenessChecker");
    }
    reactor->schedule_timer(checker, 0, check_interval, check_interval);
    cout << "Waiting for director..." << endl;
}

int Producer::handle_input(ACE_HANDLE h){
    //once quit_flag is set, no longer handle keyboard event
    if(quit_flag)
        return -1;

    char buf[BUFSIZ];
    string str;
    if(h == ACE_STDIN){
        ssize_t result = ACE_OS::read(h, buf, BUFSIZ);
        if(result <= 0)
            return -1;
        if(buf[0] == '\n')
            return 0;
        for(size_t i=0; i<strlen(buf); i++){
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

    int send_token = 0;
    //to quit the producer, the producer first send <quit> command to all connected
    if(str_split.front().compare("quit") == 0){
        SignalHandler::set_quit_flag();
        if(close() < 0)
            throw runtime_error("Failed to quit.");
        return 0;
    }

    if(str_split.size() != 2){
        cerr << "Format: play <id-of-script>" << endl;
        return -1;
    }

	int status = playlist->find(str_split.back(), stream);
    if(status == PlayList::NOT_FOUND){
        cerr << "Invalid script id!" << endl;
        return -1;
    }
    ACE_INET_Addr remote_addr;
    stream->get_remote_addr(remote_addr);
    char addr_buffer[BUFSIZ];
    remote_addr.addr_to_string(addr_buffer, BUFSIZ);

    string id_converted = playlist->convertId(str_split.back());
    if(id_converted.size() == 0){
        cerr << "Invalid script id" << endl;
        return -1;
    }

    if(str_split.front().compare("play") == 0){
		if(status == PlayList::PLAYING){
			cerr << "Script now playing..."<<endl;
			return -1;
		}
        command = Protocal::composeCommand(Protocal::P_PLAY, id_converted, port);
    }else if(str_split.front().compare("stop") == 0){
		if(status == PlayList::VALID){
			cerr << "Script is not playing." << endl;
			return -1;
		}
        command = Protocal::composeCommand(Protocal::P_STOP, id_converted, port);
    }else{
        cerr << "Invalid command." << endl;
        return -1;
    }

    send_token = Sender::sendMessage(command, *stream);
    if(send_token < 0){
        cerr << "Connection to " << addr_buffer << "lost, remote all scripts from it" << endl;
        playlist->removeAddr(remote_addr);
        cout << "Current list:" << endl;
        playlist->printList();
    }
    return send_token;
}

int Producer::close(){
    //only handle close event once
    if(playlist->is_empty()){
        SignalHandler::interrupt();
        return 0;
    }
    string command = Protocal::composeCommand(Protocal::P_QUIT, string(""), port);
    vector<ACE_INET_Addr> addr_to_remove;
    list<ItemSet> data = playlist->getList();
    for(const auto&v :data){
        if(Sender::sendMessage(command, *v.stream) < 0)
            addr_to_remove.push_back(v.getAddr());
    }
    for(const auto&v: addr_to_remove){
        playlist->removeAddr(v);
    }

    if(playlist->is_empty()){
        SignalHandler::interrupt();
        return 0;
    }
    cout << "Waiting following client to quit: " << endl;
    playlist->printAddress();

    //wait for check_interval, register a timer event
    QuitTimer* h = nullptr;
    ACE_NEW_RETURN(h, QuitTimer(playlist), -1);
    if(reactor->schedule_timer(h, 0, check_interval) < 0)
        return -1;
    return 0;
}