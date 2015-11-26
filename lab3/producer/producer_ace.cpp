#include "producer_ace.h"
#include "../string_util.h"
using namespace std;

int ProducerInputHandler::handle_input(ACE_HANDLE h){
    char data;
    ACE_SOCK_Stream& stream = peer();
    string str;
    while(true){
        ssize_t res = stream.recv(&data,1);
        if(res <= 0)
            break;
        str.append(&data);
    }
    stream.close();
    return parseCommand(str);
}

int ProducerInputHandler::parseCommand(const std::string &str) {
    cout << "command: "<< str << endl;
    vector<string> str_split;
    string_util::split_str(str, str_split);
    vector<string> play_title;
    string str_play_title;

    string templatestr("<list>");

    //compose the substring containing all play titles
    for(int i=1; i<str_split.size()-1; ++i)
        str_play_title.append(str_split[i]);
    cout << str_play_title << endl;
    for(const auto&v : str_split[0])
        cout << 't'<< v;
    cout <<'c' << endl;
    for(const auto&v : str_split)
        cout << v.size() << ' ';
    cout << endl;

    if(str_split[0].compare(templatestr) == 0){
        string temp;
        bool is_title = false;
        for(const auto&v: str_play_title){
            cout << v << endl;
            if(v == '('){
                is_title = true;
            }else if(v == ')') {
                play_title.push_back(temp);
                temp.clear();
                is_title = false;
            }else{
                if(is_title)
                    temp.append(&v);
            }
        }

        for(const auto&v: play_title)
            cout << v << endl;

    }else if(str_split[0].compare("<quit>") == 0){

    }else if(str_split[0].compare("<play>") == 0){

    }else{
        cout << "protocal error, omit command: " << str_split[0] << endl;
        return -1;
    }

    return 0;
}