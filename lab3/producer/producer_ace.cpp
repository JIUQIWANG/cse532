#include "producer.h"
#include "../string_util.h"
using namespace std;

int ProducerInputHandler::handle_input(ACE_HANDLE h){
    char data;
    ACE_SOCK_Stream& stream = peer();
    string str;
    while(true){
        ssize_t res = stream.recv(&data,1);
        if(res <= 0)
            break;
        str.append(&data);
    }
    cout << str << endl;
    data = 'K';
    if(stream.send(&data,1) < 0){
        return -1;
    }
    cout << "Response sent" << endl;
    stream.close();
    return 0;
}