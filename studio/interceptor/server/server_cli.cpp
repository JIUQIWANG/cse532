#include <iostream>
#include "server_acceptor.h"
#include "../signal_handler.h"
using namespace std;

enum returnType{SUCCESS,E_ARGUMENT,E_REACTOR,E_CONNECTION,E_MEMORY, E_OTHER};

int main(int argc, char** argv){
    if(argc > 1){
        cerr << "Invalid arguments!"<<endl;
        return returnType::E_ARGUMENT;
    }
    cout << argv[0] << endl;
    const unsigned short port = 2000;
    const int arg_modulus = 10;
    const int arg_rot = 20;

    shared_ptr<EndPoint> endpoint(new EndPoint());

    Interceptor* int_pass_ptr = new Interceptor_Pass();
    shared_ptr<Interceptor> int_pass(int_pass_ptr);
    Interceptor* int_modulus_ptr  = new Interceptor_Modular(arg_modulus);
    shared_ptr<Interceptor> int_modulus(int_modulus_ptr);
	Interceptor* int_modulus_ptr2  = new Interceptor_Modular(3);
    shared_ptr<Interceptor> int_modulus2(int_modulus_ptr2);
    Interceptor* int_rot_ptr = new Interceptor_Rot(arg_rot);
    shared_ptr<Interceptor> int_rot(int_rot_ptr);

	//endpoint->add(int_pass);
    endpoint->add(int_modulus);
	endpoint->add(int_modulus2);
    //endpoint->add(int_rot);


    ACE_INET_Addr local_addr(2000);
    Server_Acceptor *acceptor;
    ACE_NEW_RETURN(acceptor, Server_Acceptor(endpoint), returnType::E_MEMORY);
    unique_ptr<Server_Acceptor> guard_acceptor(acceptor);

    if(acceptor->open(local_addr) < 0){
        cerr << "Can not open acceptor"<<endl;
        return returnType::E_CONNECTION;
    }

    SignalHandler *signalHandler;
    ACE_NEW_RETURN(signalHandler, SignalHandler(), returnType::E_MEMORY);
    unique_ptr<SignalHandler> guard_signal(signalHandler);
    if(ACE_Reactor::instance()->register_handler(SIGINT, signalHandler) < 0){
        cerr << "Can not register signal handler" << endl;
        return returnType::E_REACTOR;
    }

    while(true){
        if(SignalHandler::is_interrupted()){
            break;
        }
        ACE_Reactor::instance()->handle_events();
    }

    guard_acceptor.release();
    guard_signal.release();
    return returnType::SUCCESS;
}
