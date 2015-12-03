#ifndef THREAD_GUARD
#define THREAD_GUARD
#include <thread>

class thread_guard{
public:
thread_guard():t(std::thread()){};
    explicit thread_guard(std::thread& t_){
	bind(t_);
    }

    void bind(std::thread& t_){
	t = std::move(t_);
    }
    void join(){
	if(t.joinable())
	    t.join();
    }
    

    ~thread_guard(){
	if(t.joinable())
	    t.join();
    }
private:
    thread_guard(thread_guard const&);
    thread_guard& operator= (thread_guard const&);
    std::thread t;
};
#endif












