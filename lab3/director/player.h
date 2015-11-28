#pragma once
#include "play.h"
#include <vector>
#include <string>
#include <thread>
#include <queue>
#include <fstream>
#include <mutex>
#include <memory>
#include <atomic>
#include <future>
#include <functional>
#include <exception>
#include <condition_variable>

#define TERMINATION_TOKEN -1

struct Part{
	Part(const int fragmentid_, const std::pair<std::string, std::string> str_) : fragmentid(fragmentid_), str(str_){};
	Part();
	//fragment id also act as termination token. When the Player object process a part with fragmentid = TERMINATION_TOKEN, it exit the loop
	int fragmentid;
	std::pair<std::string, std::string> str;
};

//We design the Player class following Active Object pattern. The calling thread add task to AO by calling enter() method. The task will be queued into working queue and been process by actPart() method from working thread.
class Player
{
public:
	Player(Play& play_);
	~Player();

	inline void get_future(std::future<int> & f){ f = move(exceptionHandler); }
	void enter(const std::shared_ptr<Part> part);
	int read();
	void exit();

	//Parse str into Line object line.
	static void parseString(const std::string& str, const std::string& character, Line& line);
private:
	Play &play;
	//Note: in our design, exit() method set 'done' flag to true and wake condition variable to exit the while loop. This only has to be done once during destructing, so we make it private and only be called from destructor.
	void act(int fragmentid);
	void actPart(const std::shared_ptr<Part>& part);

	std::deque<std::shared_ptr<Part> > working_queue;
	std::vector<Line> lines;
	std::thread t;

	//We store the future of working thread to handle exception. The exception thrown in the working thread will be re-thrown in the main thread.
	mutable std::future<int> exceptionHandler;
	mutable std::mutex mt;
	mutable std::condition_variable cv;
};


