#include "common.h"

using namespace std;

list<ServiceItem> Service_Logger::svc_set;
mutex Service_Logger::mt;

void Service_Logger::registerService(ACE_Service_Object*ptr, const string& label){
    lock_guard<mutex> guard(mt);
    for(const auto&v: svc_set){
        if(v.ptr == ptr){
            cerr << "Service_Logger::registerService(): Service registered" << endl;
            return;
        }
    }
    stringstream ss;
    ss << static_cast<const void*>(ptr);
    string ptr_str(ss.str());
    ServiceItem item(ptr, ptr_str, label);
    svc_set.push_back(item);
    printList();
}

void Service_Logger::changeLabel(ACE_Service_Object* ptr, const  string& new_label){
    lock_guard<mutex> guard(mt);
    for(const auto&v: svc_set){
        if(v.label.compare(new_label) == 0){
            cerr << "Service_Logger::changeLabel(): Label already in use: " << endl;
            cerr << v.ptr << ' ' << v.label << endl;
            return;
        }
    }
    for(auto&v: svc_set){
        if(v.ptr == ptr){
            v.label = new_label;
            printList();
            return;
        }
    }
    cerr << "Service_Logger::changeLabel(): Service not found" << endl;
}

ACE_Service_Object* Service_Logger::retriveService(const std::string &str) {
    ACE_Service_Object* ptr = 0;
    lock_guard<mutex> guard(mt);
    for(auto& v: svc_set){
        if(v.ptr_str.compare(str) == 0 || v.label.compare(str) == 0) {
            ptr = v.ptr;
            break;
        }
    }
    return ptr;
}

void Service_Logger::printList(){
	cout << "---------------------------------"<<endl;
    cout << "Current Registered Service Object:"<<endl;
    for(const auto&v: svc_set)
	cout << v.ptr << ' '<< v.label << endl << flush;
}

void Service_Logger::unregister(ACE_Service_Object *ptr) {
    list<ServiceItem>::iterator iter = svc_set.begin();
    for(; iter!=svc_set.end(); ++iter){
        if(iter->ptr == ptr) {
            svc_set.erase(iter);
            break;
        }
    }
}
