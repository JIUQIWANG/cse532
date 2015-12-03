#ifndef COMPONENT_CONFIGURATOR_COMMON_H
#define COMPONENT_CONFIGURATOR_COMMON_H

#include <ace/Service_Object.h>
#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <mutex>
#include "string_util.h"

class Service_Logger{
public:
    static void registerService(ACE_Service_Object* ptr, const std::string& label);
    static void unregister(ACE_Service_Object* ptr);
    static void changeLabel(ACE_Service_Object* ptr, const std::string& new_label);
    static ACE_Service_Object* retriveService(const std::string& str);
private:
    static void printList();
    static std::map<ACE_Service_Object*, std::string> svc_set;
    static std::mutex mt;
};

#endif
