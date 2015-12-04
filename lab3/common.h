#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <iostream>
#include <vector>
#include "string_util.h"

#ifdef _WIN32
#define CLEAN_SCREEN system("cls")
#else
#define CLEAN_SCREEN system("clear")
#endif

enum returnType{SUCCESS,E_ARGUMENT,E_REACTOR,E_CONNECTION,E_MEMORY, E_OTHER};

#define ERROR_RETURN -1
#define SUCCESS_RETURN 0

const unsigned short default_port = 2000;
//if the address argument of director is dummy_address, assign ACE_LOCALHOST
const std::string dummy_address("0.0.0.0");

class Protocal{
public:
    enum protocalType{
        P_LIST,
        P_PLAY,
        P_PLAYING,
		P_FINISH,
        P_STOP,
        P_CHECK,
        P_QUIT
    };

    static inline void printInstruction(){
        std::cout << "Operations\t play <id-of-script>\t stop <id-of-script>\t quit" << std::endl;
        std::cout << "---------------------------" << std::endl;
    }

    static int parseCommand(const std::string& str, protocalType& type, std::vector<std::string>& arguments, unsigned short& remote_port);

    static std::string composeCommand(const protocalType& type, const std::vector<std::string>& arguments, const unsigned short local_port);

    static std::string composeCommand(const protocalType& type, const std::string& str, const unsigned short local_port);

};
#endif
