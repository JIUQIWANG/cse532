#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <mutex>
#include <atomic>
#include <condition_variable>

//struct for lines
struct Line{
	Line() :id(-1){}
	int id;
	std::string character;
	std::string text;
};

//struct for different situations which returned from enter and exit function of play class
enum Situation{
	failEnter = 1,
	successEnter = 2,
	failExit = 3,
	successExit = 4,
	interrupted = 5
};

class Play
{
public:
	Play(std::vector<std::string>& names_) :start(names_.begin()), cur(names_.begin()), end(names_.end()), line_counter(1), scene_fragment_counter(0), on_stage(0), is_sorted(false){
		interrupt_flag.store(false);
		stop_flag.store(false);
		if (names_.size() > 0){
			//std::cout << (*cur) << std::endl;
			cur++;
		}
	}
	~Play(){}

	//recite function same as previous labs except one more argument
	//enter and exit functions which are requested in lab2
	void recite(std::vector<Line>::iterator& iter, int fragment);
	Situation enter(int enter_scene);
	Situation exit();
	void reset();

	//set interrupt_flag
	void interrupt();
	void stop();
private:
	//end stores names_.end() which is used to judge the last element when iterating
	std::vector<std::string>::iterator cur;
	const std::vector<std::string>::iterator start;
	const std::vector<std::string>::iterator end;
	std::string name;
	std::string current_character;
	int scene_fragment_counter;
	int line_counter;
	int on_stage;
	bool is_sorted;
	std::atomic<bool> stop_flag;
	std::atomic<bool> interrupt_flag;
	mutable std::mutex mt;
	mutable std::condition_variable cv;
	mutable std::mutex enter_exit_lock;
	mutable std::condition_variable enter_exit_cv;
};


