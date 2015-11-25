#include "producer.h"

using namespace std;

int ProducerInputHandler::handle_input(ACE_HANDLE h){
    char data;
    ACE_SOCK_Stream& stream = peer();
    while(true){
	int res = stream.recv(&data,1);
	if(res <= 0)
	    break;
	cout << data;
    }
    stream.close();
    cout<<endl;
    return 0;
}