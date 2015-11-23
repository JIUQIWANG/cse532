#include "producer.h"

using namespace std;

Producer::Producer(const unsigned short port_, ACE_Reactor* reactor_): port(port_), reactor(reactor_), playlist(), acceptor(playlist), connector(){
//    playlist = shared_ptr<PlayList>(new PlayList());
//    acceptor = shared_ptr<ProducerAcceptor>(new ProducerAcceptor(playlist));
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
    ACE_INET_Addr addr(port, ACE_LOCALHOST);
    ProducerOutputHandler* h;
    ACE_NEW_RETURN(h, ProducerOutputHandler(), -1);
    connector.connect(h, addr);
    if(h->sendMessage(str) < 0)
	return -1;
    connector.close();
    return 0;
}

void Producer::close(){
    
}
