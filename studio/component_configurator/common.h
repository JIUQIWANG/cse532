#ifndef COMPONENT_CONFIGURATOR_COMMON_H
#define COMPONENT_CONFIGURATOR_COMMON_H

#include <ace/Service_Object.h>
#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include <vector>
#include <mutex>
#include "string_util.h"

struct ServiceItem{
    ServiceItem(): ptr(0), ptr_str(""), label(""){}
    ServiceItem(ACE_Service_Object* ptr_,std::string ptr_str_, std::string label_):
    ptr(ptr_), ptr_str(ptr_str_), label(label_){}

    ACE_Service_Object* ptr;
    std::string ptr_str;
    std::string label;
};

class Service_Logger{
public:
    static void registerService(ACE_Service_Object* ptr, const std::string& label);
    static void unregister(ACE_Service_Object* ptr);
    static void changeLabel(ACE_Service_Object*, const std::string& new_label);
    static ACE_Service_Object* retriveService(const std::string& str);
private:
    static void printList();
    static std::list<ServiceItem> svc_set;
    static std::mutex mt;
};

#endif
