#include "producer.h"

using namespace std;

int ProducerInputHandler::handle_input(ACE_HANDLE h){
    char data;
    const ACE_SOCK_Stream& stream = peer();
    while(true){
	if(stream.recv(&data, 1) <= 0)
	    break;
	cout << data;
    }
    cout<<endl;
    return 0;
}
