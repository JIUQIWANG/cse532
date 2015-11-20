//
// Created by yanhang on 11/18/15.
//

#include "client.h"
#include <ace/INET_Addr.h>
#include <ace/SOCK_Connector.h>

using namespace std;

Client::Client(int argc, char **argv, const unsigned short port_):
        port(port_){
    messages.resize(argc - 1);
    for(int i=0; i<messages.size(); i++){
        messages[i] = string(argv[i+1]);
    }
}

int Client::handle_timeout(const ACE_Time_Value &value, const void *pVoid) {
    ACE_INET_Addr addr(port, ACE_LOCALHOST);
    ACE_SOCK_Connector connector;
    ACE_SOCK_Stream stream;
    if(connector.connect(stream,addr) < 0){
        cerr << "Can not connect to server!" <<endl;
        return 0;
    }

    char space = ' ';
    for(int i=0; i<messages.size(); i++) {
        if (stream.send(messages[i].c_str(), messages[i].size()) < 0) {
            cerr << "Can not send message" << endl;
            return 0;
        }
        if (stream.send(&space,1) < 0) {
            cerr << "Can not send message" << endl;
            return 0;
        }
    }
    stream.close();
    return 1;
    //return ACE_Event_Handler::handle_timeout(current_time, act);
}
