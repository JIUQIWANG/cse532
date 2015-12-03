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
    const int arg_modulus = 5;
    const int arg_rot = 10;

    ACE_INET_Addr addr(port, ACE_LOCALHOST);

    //create EndPoint and register interceptor
    shared_ptr<EndPoint> endpoint(new EndPoint());

    Interceptor* int_pass_ptr = new Interceptor_Pass();
    shared_ptr<Interceptor> int_pass(int_pass_ptr);
    Interceptor* int_modulus_ptr  = new Interceptor_Modular(arg_modulus);
    shared_ptr<Interceptor> int_modulus(int_modulus_ptr);
    Interceptor* int_rot_ptr = new Interceptor_Rot(arg_rot);
    shared_ptr<Interceptor> int_rot(int_rot_ptr);

    endpoint->add(int_pass);
    endpoint->add(int_modulus);
    endpoint->add(int_rot);

    //use unique_ptr to avoid memory leak
    PeriodSender* sender;
    ACE_NEW_RETURN(sender, PeriodSender(argc, argv, endpoint), -1);

    ClientConnector *client;
    ACE_NEW_RETURN(client, ClientConnector(), -1);
    unique_ptr<ClientConnector> guard_connector(client);

    if(guard_connector->connect(sender, addr) < 0) {
        cerr << "Can not connect to server" << endl;
        return -1;
    }

    SignalHandler *signalHandler;
    ACE_NEW_RETURN(signalHandler, SignalHandler(), -1);
    unique_ptr<SignalHandler> guard_signal(signalHandler);
    if(ACE_Reactor::instance()->register_handler(SIGINT, signalHandler) < 0){
        cerr << "Can not register signal handler" << endl;
        return -1;
    }

    while(true){
        if(SignalHandler::is_interrupted()){
            break;
        }
        ACE_Reactor::instance()->handle_events();
    }

    guard_signal.release();

    return returnType::success;
}
