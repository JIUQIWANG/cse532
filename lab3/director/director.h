#ifndef DIRECTOR_H
#define DIRECTOR_H

#include <string>
#include <thread>
#include <mutex>
#include <memory>
#include <iostream>
#include <fstream>
#include <exception>
#include <stdexcept>
#include <future>
#include <vector>
#include <deque>
#include <atomic>

class Play;
class Player;
struct Part;

//Scene is composed by one or more Fragments. Each fragment owns particular players.
struct Fragment{
	Fragment(){};
	std::vector <std::shared_ptr<Part> > parts;
};

struct Scene{
	Scene(){}
	std::vector<std::shared_ptr<Fragment> > fragments;
};

class Director{
public:
	Director(unsigned int minimum_num_players_, const std::vector<std::string> scripts_);
	~Director();

	//cue(int id) will try to play the script whose ID is id
	//stop(int id) will stop the corresponding script play
	//parseCommand() will try to parse the command and call corresponding functions like cue() and stop()
	//getPlayList() will return the available scripts
	//work() is a loop to pull available play works from play_queue and call cue() function
	void cue(int id);
	std::string stop(int id);
	std::string parseCommand(const std::string& command);
	std::string getPlayList() const;
	int work();
	inline void quit(){

	}
private:
	//isOverride is the variable for extra part 2
	//num_players is the proper thread pool's size which is calculated by comparison of minimum_num_players and max_players_consecutive
	bool isOverride;
	unsigned int minimum_num_players;
	unsigned int max_players_consecutive;
	unsigned int num_players;

	//storeScript() will initialize corresponding member variables given script_name_
	void storeScript(const std::string script_name_, int position);
	//Member variables are changed a little bit compared to previous lab2
	//Most of them are 2-dimension vectors now such as we have plays instead of play compared to previous lab
	//scene_titles stores the name of the scene
	//scenes stores all fragments
	//players is a thread pool whose size is num_players which is calculated in constructor
	std::vector<std::string> scripts;
	std::vector<std::vector<std::string> > scene_titles;
	std::vector<std::vector<Scene> > scenes;
	std::vector<std::shared_ptr<Play> > plays;
	std::vector<std::shared_ptr<Player> > players;
	std::vector<std::future<int> > exceptionHandlers;
	std::deque<int> play_queue;
	mutable std::future<int> workException;
	std::thread workplay;
	mutable std::mutex mt;
	mutable std::mutex mu;
	mutable std::condition_variable cv;
	std::atomic<bool> isFinished;
};

#endif

