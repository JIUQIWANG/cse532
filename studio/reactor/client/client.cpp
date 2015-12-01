//
// Created by yanhang on 11/18/15.
//

#include "client.h"
#include "../socket_handler.h"
using namespace std;

Client::Client(int argc, char **argv, const ACE_SOCK_Stream& stream_):
        counter(0),stream(stream_){
    messages.resize(argc - 1);
    for(int i=0; i<messages.size(); i++){
        messages[i] = string(argv[i+1]);
    }
}

int Client::handle_timeout(const ACE_Time_Value &value, const void *pVoid) {
    printf("sending data...");
    char space = ' ';
    char numbuffer[10];

    sprintf(numbuffer, "%d ", counter++);
    if (stream.send_n(numbuffer, strlen(numbuffer)) < 0) {
        cerr << endl << "Can not send message" << endl;
        return -1;
    }
    for(int i=0; i<messages.size(); i++) {
        if (stream.send_n(messages[i].c_str(), messages[i].size()) < 0) {
            cerr << endl << "Can not send message" << endl;
            return -1;
        }
        if (stream.send_n(&space,1) < 0) {
            cerr << endl << "Can not send message" << endl;
            return -1;
        }
    }
    printf("data sent\n");
    return 0;
}
