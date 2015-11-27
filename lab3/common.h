#ifndef COMMON_H
#define COMMON_H
#include <string>
#include <iostream>
#include <vector>
#include "string_util.h"

enum returnType{
    SUCCESS,
    EARGUMENT,
    EREACTOR,
    ECONNECTION,
    EMEMORY,
    EOTHER
};

const unsigned short default_port = 2000;

class Protocal{
public:
    enum protocalType{
        P_LIST,
        P_PLAY,
        P_OCCUPIED,
        P_STOP,
        P_QUIT
    };

    static inline void printInstruction(){
        std::cout << "Operations" << std::endl;
        std::cout << "play <id-of-script>" << std::endl;
        std::cout << "stop <id-of-script>" << std::endl;
        std::cout << "quit" << std::endl;
        std::cout << "---------------------------" << std::endl;
    }

    static int parseCommand(const std::string& str, protocalType& type, std::vector<std::string>& arguments, unsigned short& remote_port);

    static std::string composeCommand(const protocalType& type, const std::vector<std::string>& arguments, const unsigned short local_port);

    static std::string composeCommand(const protocalType& type, const std::string& str, const unsigned short local_port);

};
#endif
