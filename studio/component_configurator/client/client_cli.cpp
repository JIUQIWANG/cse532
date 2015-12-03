#include "client.h"
#include "../signal_handler.h"
#include "../thread_guard.h"
#include <exception>
#include <ace/Time_Value.h>
#include <iostream>
#include <string>
using namespace std;

enum returnType{
    success,
    invalid_argument,
    other
};

void logging_thread(int argc, ACE_TCHAR* argv[]){
    while(!SignalHandler::is_interrupted()){
        cout << ">>";
        string str;
        std::getline(std::cin, str);
        vector<string> str_split;
        string_util::split_str(str, str_split);

        ACE_Service_Object* ptr = 0;
        try{
            long addr = stol(str_split[1], 0, 16);
            cout << "addr:" << addr << endl;
            ptr = (ACE_Service_Object*)addr;

        }catch (const std::exception& e){
            ptr = Service_Logger::retriveService(str_split[1]);
            cout << "ptr:" << ptr << endl;
            if(!ptr) {
                cerr << "Service not found" << endl;
                continue;
            }
        }

        if(str_split.front().compare("rename") == 0){
            if(str_split.size() != 3){
                cout << "Invalid argument" << endl;
                continue;
            }
            Service_Logger::changeLabel(ptr, str_split.back());
        }else if(str_split.front().compare("init") == 0){
            if(str_split.size() != 2){
                cout << "Invalid argument" << endl;
                continue;
            }
            if(ptr->init(argc, argv) < 0)
                cerr << ptr << " init failed" << endl;
            else
                cout << ptr << " inited" << endl;
        }else if(str_split.front().compare("suspend") == 0){
            if(str_split.size() != 2){
                cout << "Invalid argument" << endl;
                continue;
            }
            ptr->suspend();
            cout << ptr << " suspended" << endl;
        }else if(str_split.front().compare("resume") == 0){
            if(str_split.size() != 2){
                cout << "Invalid argument" << endl;
                continue;
            }
            ptr->resume();
            cout << ptr << " resumed" << endl;
        }else if(str_split.front().compare("fini") == 0) {
            if (str_split.size() != 2) {
                cout << "Invalid argument" << endl;
                continue;
            }
            ptr->fini();
            cout << ptr << " finished" << endl;
        }else if(str_split.front().compare("info") == 0){
            if (str_split.size() != 2) {
                cout << "Invalid argument" << endl;
                continue;
            }
            ACE_TCHAR** info_str;
            ptr->info(info_str ,0);
        }else{
            cerr << "Invalid argument" << endl;
        }
    }

}

int ACE_TMAIN(int argc, ACE_TCHAR* argv[]){
    ClientConnector *client;
    ACE_NEW_RETURN(client, ClientConnector(), -1);
    //unique_ptr<ClientConnector> guard_connector(client);

    SignalHandler *signalHandler;
    ACE_NEW_RETURN(signalHandler, SignalHandler(), -1);
    unique_ptr<SignalHandler> guard_signal(signalHandler);
    if(ACE_Reactor::instance()->register_handler(SIGINT, signalHandler) < 0){
        cerr << "Can not register signal handler" << endl;
        return -1;
    }

    thread t(logging_thread, argc, argv);
    thread_guard t_guard(t);

    while(true){
        if(SignalHandler::is_interrupted()){
            break;
        }
        ACE_Reactor::instance()->handle_events();
    }

    guard_signal.release();

    return returnType::success;
}
