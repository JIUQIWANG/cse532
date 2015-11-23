//
// Created by yanhang on 11/18/15.
//

#include "client.h"
#include "../socket_handler.h"
using namespace std;

Client::Client(int argc, char **argv, const unsigned short port_):
    port(port_), counter(0){
    messages.resize(argc - 1);
    for(int i=0; i<messages.size(); i++){
        messages[i] = string(argv[i+1]);
    }
}

int Client::handle_timeout(const ACE_Time_Value &value, const void *pVoid) {
    printf("sending data\n");
	   
    ACE_INET_Addr addr(port, ACE_LOCALHOST);
    ACE_SOCK_Connector connector;
    ACE_SOCK_Stream stream;
    if(connector.connect(stream,addr) < 0){
        cerr << "Can not connect to server!" <<endl;
        return -1;
    }

    char space = ' ';
    char numbuffer[10];
    
    sprintf(numbuffer, "%d", counter++);
    if (stream.send(numbuffer, strlen(numbuffer)) < 0) {
	cerr << "Can not send message" << endl;
	return -1;
    }
    for(int i=0; i<messages.size(); i++) {
        if (stream.send(messages[i].c_str(), messages[i].size()) < 0) {
            cerr << "Can not send message" << endl;
            return -1;
        }
        if (stream.send(&space,1) < 0) {
            cerr << "Can not send message" << endl;
            return -1;
        }
    }
    printf("data sent\n");
    stream.close();
    cout << "stream closed"<<endl;
    return 0;
}

// int Client::handle_close(ACE_HANDLE f, ACE_Reactor_Mask m){
//     cout << "Client::handle_close() is called"<<endl<<flush;
//     return 0;
// }
