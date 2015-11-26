#ifndef COMMON_H
#define COMMON_H
#include <string>
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
        cout << "Operations" << endl;
        cout << "play <id-of-script>" << endl;
        cout << "stop <id-of-script>" << endl;
        cout << "quit" << endl;
        cout << "---------------------------" << endl;
    }

    static int parseCommand(const std::string& str, protocalType& type, std::vector<std::string>& arguments, unsigned short& remote_port){
        std::vector<std::string> str_split;
        string_util::split_str(str, str_split);

        cout << "c1 " << str << endl;
        //extract remote port number
        for(const auto& v: str_split.back()){
            if(v < '0' || v > '9'){
                cerr << "Protocal::parseCommand(): invalid port number: " << str_split.back();
                return -1;
            }
        }
        remote_port = (unsigned short)std::stoi(str_split.back());

        std::string mid;
        //compose the substring containing all play titles
        for(int i=1; i<str_split.size()-1; ++i)
            mid.append(str_split[i]);

        if(str_split[0].compare("<list>") == 0){
            type = P_LIST;
            int startid = -1;
            for(int i=0; i<mid.size(); i++)
                if(mid[i] == '('){
                    startid = i + 1;
                }else if(mid[i] == ')') {
                    if (startid == -1) {
                        cerr << "Protocal::parseCommand(): Mismatched parentheses!" << endl;
                        return -1;
                    }
                    arguments.push_back(mid.substr(startid, i - startid));
                    startid = -1;
                }
            if(startid != -1){
                cerr << "Protocal::parseCommand(): Mismatched parentheses!" << endl;
                return -1;
            }
        }else if(str_split[0].compare("<quit>") == 0){
            type = P_QUIT;
        }else if(str_split[0].compare("<play>") == 0){
            type = P_PLAY;
            arguments.push_back(mid);
        }else if(str_split[0].compare("<occupied>") == 0){
            type = P_OCCUPIED;
            arguments.push_back(mid);
        }else if(str_split[0].compare("<stop>") == 0){
            type = P_STOP;
            arguments.push_back(mid);
        }else{
            cout << "Protocal::parseCommand(): protocal error, omit command: " << str_split[0] << endl;
            return -1;
        }
        return 0;
    }

    static std::string composeCommand(const protocalType& type, const std::vector<std::string>& arguments, const unsigned short local_port){
        std::string str;
        switch(type){
            case P_LIST:
                str.append("<list> ");
                break;
            case P_PLAY:
                str.append("<play> ");
                break;
            case P_OCCUPIED:
                str.append("<occupied> ");
                break;
            case P_STOP:
                str.append("<stop> ");
                break;
            case P_QUIT:
                str.append("<quit> ");
                break;
            default:
                cerr << "Protocal::composeCommand(): invalid command type!" << endl;
                str = "<invalid> " + std::to_string(local_port);
                return str;
        }
        for(const auto& v: arguments)
            str.append(v + " ");
        str.append(std::to_string(local_port));
        return str;
    }

    static std::string composeCommand(const protocalType& type, const std::string& str, const unsigned short local_port){
        std::vector<std::string> arguments;
        arguments.push_back(str);
        return composeCommand(type, arguments, local_port);
    }
};
#endif
