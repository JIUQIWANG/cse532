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

#include "../outputHandler.h"
#include "../common.h"
#include "play.h"
#include "player.h"

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
	void cue(int id);
	void stop(int id);
	void parseCommand(const std::string& command);
	std::string getPlayList();
private:
	//isOverride is the variable for extra part 2. num_players is the proper thread pool's size which is calculated by comparison of minimum_num_players adn max_players_consecutive
	bool isOverride;
	unsigned int minimum_num_players;
	unsigned int max_players_consecutive;
	unsigned int num_players;

	void storeScript(const std::string script_name_, int position);
	//scene_titles stores the name of the scene
	//scenes stores all fragments
	//players is a thread pool whose size is num_players which is calculated in constructor
	std::vector<std::string> scripts;
	std::vector<std::vector<std::string> > scene_titles;
	std::vector<std::vector<Scene> > scenes;
	std::vector<std::shared_ptr<Play> > plays;
	std::shared_ptr<Play> play;
	std::vector<std::shared_ptr<Player> > players;
	std::vector<std::future<int> > exceptionHandlers;
	std::vector<std::future<std::string> > infos;
};

#endif

