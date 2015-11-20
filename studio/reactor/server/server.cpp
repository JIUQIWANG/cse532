#include <iostream>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Connector.h>
#include <ace/SOCK_Stream.h>

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
    ACE_INET_Addr addr(port, ACE_LOCALHOST);
    ACE_SOCK_Acceptor acceptor;
    ACE_SOCK_Stream stream;

    while(true) {
        if(acceptor.open(addr, 1) < 0){
            cerr << "Unable to open acceptor." << endl;
            return returnType::other;
        }
        ACE_TCHAR addr_buffer[1024];
        addr.addr_to_string(addr_buffer,1024);
        cout << "acceptor opened, address:" << addr_buffer << endl;
        if (acceptor.accept(stream) < 0) {
            cerr << "acceptor cannot accept socket connection" << endl;
            return returnType::other;
        }
        while (true) {
            ACE_TCHAR c;
            if (stream.recv(&c, 1) <= 0)
                break;
            cout << c;
        }
        acceptor.close();
        cout<<endl<<"Connection closed"<<endl;
    }
    return returnType::success;
}