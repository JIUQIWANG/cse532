#include <iostream>
#include <ace/ACE.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Connector.h>

using namespace std;

enum returnType{
    success,
    invalid_argument,
    other
};

int main(int argc, char** argv){
    if(argc < 2){
        cerr << "Invalid argument!"<<endl;
        return returnType::invalid_argument;
    }
    const unsigned short port = 2000;
    ACE_INET_Addr addr(port, ACE_LOCALHOST);
    ACE_SOCK_Connector connector;
    ACE_SOCK_Stream stream;

    if(connector.connect(stream,addr) < 0){
        cerr << "Can not connect to server!" <<endl;
        return returnType::other;
    }

    char space = ' ';
    for(int i=1; i<argc; i++) {
        if (stream.send(argv[i], strlen(argv[i])) < 0) {
            cerr << "Can not send message" << endl;
            return returnType::other;
        }
        if (stream.send(&space,1) < 0) {
            cerr << "Can not send message" << endl;
            return returnType::other;
        }
    }
    stream.close();
    return returnType::success;
}