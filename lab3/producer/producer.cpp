#include "producer.h"

using namespace std;

//check connection of all directors in every 3 seconds
const ACE_Time_Value Producer::check_interval(3, 0);

int QuitTimer::handle_timeout(const ACE_Time_Value &current_time, const void *act){
    cout << "No response from the following scripts:" << endl;
    playlist->printList();
    cout << "Force quit" << endl;
    QuitFlags::interrupt();
    return ERROR_RETURN;
}

Producer::Producer(const unsigned short port_, ACE_Reactor* reactor_): port(port_), reactor(reactor_), playlist(new PlayList()), acceptor(nullptr), liveness_checker(nullptr){
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
    ACE_NEW_NORETURN(liveness_checker, LivenessChecker(playlist));
    if(liveness_checker == nullptr){
        throw runtime_error("Producer::Producer(): Failed to allocate LivenessChecker");
    }
    if(reactor->schedule_timer(liveness_checker, 0, check_interval, check_interval) < 0){
		throw runtime_error("Producer::Producer():Failed to regietser liveness checker");
	}
	CLEAN_SCREEN;
    cout << "Waiting for director..." << endl;
}

int Producer::handle_input(ACE_HANDLE h){
    //once quit_flag is set, no longer handle keyboard event
	if(QuitFlags::is_quit() || playlist->is_cleaning())
        return SUCCESS_RETURN;
	char buf[BUFSIZ] = {};
	string str;
    if(h == ACE_STDIN){
        ssize_t result = ACE_OS::read(h, buf, BUFSIZ);
        if(result <= 0)
            return ERROR_RETURN;
        if(buf[0] == '\n')
            return SUCCESS_RETURN;
        for(size_t i=0; i<strlen(buf); i++){
            if(buf[i] == '\n')
                break;
            str += buf[i];
        }
#ifdef _WIN32
		//on windows system, the last character of str is \n, we have to remove it before parsing command
		str.pop_back();
#endif
        handleKeyboard(str);
    }
    return SUCCESS_RETURN;
}

int Producer::handleKeyboard(const string& str){
    vector<string> str_split;
    string_util::split_str(str, str_split);
    string command;
    shared_ptr<ACE_SOCK_Stream> stream;

    updateScreen();

    int send_token = SUCCESS_RETURN;

    //to quit the producer, the producer first send <quit> command to all connected
    if(str_split.front().compare("quit") == 0){
        if(close() < 0)
            throw runtime_error("Failed to quit.");
        return ERROR_RETURN;
    }

    if(str_split.size() != 2){
        cerr << "Format: play <id-of-script>" << endl;
        return ERROR_RETURN;
    }

	int status = playlist->find(str_split.back(), stream);
    if(status == PlayList::NOT_FOUND){
        cerr << "Invalid script id!" << endl;
        return ERROR_RETURN;
    }

    ACE_INET_Addr remote_addr;
    stream->get_remote_addr(remote_addr);
    char addr_buffer[BUFSIZ];
    remote_addr.addr_to_string(addr_buffer, BUFSIZ);

    string id_converted = playlist->convertId(str_split.back());
    if(id_converted.size() == 0){
        cerr << "Invalid script id" << endl;
        return ERROR_RETURN;
    }
    if(str_split.front().compare("play") == 0){
		if(status == PlayList::PLAYING){
			cerr << "Director "<<addr_buffer<<" is playing a script, please wait until finishing..."<<endl;
			return ERROR_RETURN;
		}
        command = Protocal::composeCommand(Protocal::P_PLAY, id_converted, port);
    }else if(str_split.front().compare("stop") == 0){
		if(status == PlayList::VALID){
			cerr << "Script is not playing." << endl;
			return ERROR_RETURN;
		}
        command = Protocal::composeCommand(Protocal::P_STOP, id_converted, port);
    }else{
        cerr << "Invalid command." << endl;
        return ERROR_RETURN;
    }

    send_token = Sender::sendMessage(command, *stream);
    if(send_token != SUCCESS_RETURN){
        cerr << "Connection to " << addr_buffer << "lost, remote all scripts from it" << endl;
        playlist->removeAddr(remote_addr);
    }
    return send_token;
}

int Producer::close(){
    if(playlist->is_empty()){
        QuitFlags::interrupt();
        return SUCCESS_RETURN;
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
        QuitFlags::interrupt();
        return SUCCESS_RETURN;
    }

    //set playlist to "clean" status. Once all director are removed, Producer will exit
    playlist->enter_cleaning();
    CLEAN_SCREEN;
    cout << "Waiting following client to quit: " << endl;
    playlist->printAddress();

    //wait for check_interval, register a timer event
    QuitTimer* h = nullptr;
    ACE_NEW_RETURN(h, QuitTimer(playlist), -1);
    if(reactor->schedule_timer(h, 0, check_interval) < 0)
        return ERROR_RETURN;
    return SUCCESS_RETURN;
}
