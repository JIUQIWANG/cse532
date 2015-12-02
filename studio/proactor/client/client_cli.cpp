#include "client.h"
#include "../signal_handler.h"
#include <ace/Time_Value.h>

using namespace std;

enum returnType{
    success,
    invalid_argument,
    other
};

int main(int argc, char** argv){
    if(argc < 2){
        cerr << "Invalid arguments!"<<endl;
        return returnType::invalid_argument;
    }
    const unsigned short port = 2000;
    ACE_INET_Addr addr(port, ACE_LOCALHOST);

    //use unique_ptr to avoid memory leak

    ClientConnector *client;
    ACE_NEW_RETURN(client, ClientConnector(argc, argv), -1);
    unique_ptr<ClientConnector> guard_connector(client);

    if(guard_connector->connect(addr) < 0) {
        cerr << "Can not connect to server" << endl;
        return -1;
    }

//    SignalHandler *signalHandler;
//    ACE_NEW_RETURN(signalHandler, SignalHandler(), -1);
//    unique_ptr<SignalHandler> guard_signal(signalHandler);

    while(true){
//        if(SignalHandler::is_interrupted()){
//            break;
//        }
        ACE_Proactor::instance()->handle_events();
    }

    //guard_signal.release();

    return returnType::success;
}
