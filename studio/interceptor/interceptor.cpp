//
// Created by yanhang on 12/3/15.
//

#include "interceptor.h"
using namespace std;

void Interceptor_Modular::apply_decode(std::string &str) {
    for (auto &v: str) {
        char basechar = 'n';
        if (islower(v)) {
            basechar = 'a';
        } else if (isupper(v)) {
            basechar = 'A';
        }
        if (basechar == 'n')
            continue;
        int temp = static_cast<int>(v - basechar);
        temp = (temp - arg + 26) % 26;
        v = static_cast<char>(temp) + basechar;
    }
}

void Interceptor_Modular::apply_encode(std::string& str) {
    for(auto&v: str){
        char basechar = 'n';
        if(islower(v)){
            basechar = 'a';
        }else if(isupper(v)){
            basechar = 'A';
        }
        if(basechar == 'n')
            continue;
        int temp = static_cast<int>(v - basechar);
        temp = (temp + arg) % 26;
        v = static_cast<char>(temp) + basechar;
    }
}

void Interceptor_Rot::apply_decode(std::string &str) {
    string str2(str);
    for(int i=0; i<str.size(); i++)
        str[i] = str2[(i+str.size()-arg)%str.size()];
}

void Interceptor_Rot::apply_encode(std::string &str) {
    string str2(str);
    for(int i=0; i<str.size(); i++)
        str[i] = str2[(i+arg)%str.size()];
}

string EndPoint::encode(const std::string &str) {
    string res = str;
    for(const auto&v: interceptor_set)
        v->apply_encode(res);
    return res;
}

string EndPoint::decode(const std::string &str) {
    string res = str;
    for(int i=interceptor_set.size()-1; i>=0; i--)
        interceptor_set[i]->apply_decode(res);
    return res;
}