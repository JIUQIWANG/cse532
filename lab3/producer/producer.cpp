#include "producer.h"
using namespace std;

Producer::Producer(const unsigned short port_): port(port_){
    playlist = shared_ptr<PlayList>(new PlayList());
    acceptor = shared_ptr<ProducerAcceptor>(new ProducerAcceptor(playlist));
}

int Producer::handleKeyboard(const string& str){
    return 0;
}

void Producer::close(){
    
}
