#include "common.h"

using namespace std;

map<ACE_Service_Object*, string> Service_Logger::svc_set;
mutex Service_Logger::mt;

void Service_Logger::registerService(ACE_Service_Object*ptr, const string& label){
    lock_guard<mutex> guard(mt);
    svc_set[ptr] = label;
    printList();
}

void Service_Logger::changeLabel(ACE_Service_Object* ptr, const string& new_label){
    lock_guard<mutex> guard(mt);
    if(svc_set.find(ptr) == svc_set.end()){
	cerr << "Service Object not registered" << endl;
	return;
    }
    svc_set[ptr] = new_label;
    printList();
}

ACE_Service_Object* Service_Logger::retriveService(const std::string &str) {
    ACE_Service_Object* ptr = 0;
    lock_guard<mutex> guard(mt);
    for(const auto& v: svc_set){
        if(v.second.compare(str) == 0)
            ptr = v.first;
    }
    return ptr;
}

void Service_Logger::printList(){
    cout << "Current Registered Service Object:"<<endl;
    for(const auto&v: svc_set)
	cout << v.first << ' '<< v.second << endl << flush;
}

void Service_Logger::unregister(ACE_Service_Object *ptr) {
    svc_set.erase(ptr);
}
