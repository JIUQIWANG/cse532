#include <iostream>
#include "server_acceptor.h"

using namespace std;

enum returnType{
    success,
    invalid_argument,
    other
};

int main(int argc, char** argv){
    if(argc > 1){
        cerr << "Invalid arguments!"<<endl;
        return returnType::invalid_argument;
    }
    cout << argv[0] << endl;
    const unsigned short port = 2000;

    Server_Acceptor acceptor(port);
    if(acceptor.open() < 0){
        cerr << "Can not open acceptor"<<endl;
        return returnType::other;
    }

    ACE_Reactor::instance()->run_reactor_event_loop();
    return returnType::success;
}
