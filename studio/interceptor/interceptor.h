//
// Created by yanhang on 12/3/15.
//

#ifndef ACCEPTOR_CONNECTOR_INTERCEPTOR_H
#define ACCEPTOR_CONNECTOR_INTERCEPTOR_H
#include <string>
#include <iostream>
#include <vector>
#include <memory>

class Interceptor {
public:
    virtual void apply_encode(std::string& str) = 0;
    virtual void apply_decode(std::string& str) = 0;
};

class Interceptor_Pass: public Interceptor{
public:
    void apply_encode(std::string& str){
        return;
    }
    void apply_decode(std::string& str){
        return;
    }
};
class Interceptor_Modular: public Interceptor{
public:
    Interceptor_Modular(const int arg_): arg(arg_){}
    void apply_encode(std::string& str);
    void apply_decode(std::string& str);
private:
    const int arg;
};

class Interceptor_Rot: public Interceptor{
public:
    Interceptor_Rot(const int arg_): arg(arg_){}
    void apply_encode(std::string& str);
    void apply_decode(std::string& str);
private:
    const int arg;
};

class EndPoint{
public:
    inline void add(std::shared_ptr<Interceptor> interceptor) {
        interceptor_set.push_back(interceptor);
    }
    std::string encode(const std::string& str);
    std::string decode(const std::string& str);
private:
    typedef std::vector<std::shared_ptr<Interceptor> > Interceptor_Set;
    Interceptor_Set interceptor_set;
};
#endif //ACCEPTOR_CONNECTOR_INTERCEPTOR_H
