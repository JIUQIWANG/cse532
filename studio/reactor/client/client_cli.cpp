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
    ACE_SOCK_Connector connector;
    ACE_SOCK_Stream stream;
    if(connector.connect(stream,addr) < 0){
        cerr << endl << "Can not connect to server!" <<endl;
        return -1;
    }

    Client *client;
    ACE_NEW_RETURN(client, Client(argc, argv, stream), -1);

    ACE_Time_Value value(3,0);
    ACE_Reactor::instance()->schedule_timer(client, 0, value, value);

    while(true){
        if(SignalHandler::is_interrupted()){
            break;
        }
        ACE_Reactor::instance()->handle_events();
    }
    return returnType::success;
}
