#ifndef DIRECTOR_H
#define DIRECTOR_H
#include <ace/ACE.h>
#include <ace/Acceptor.h>
#include <ace/Connector.h>
#include <ace/Svc_Handler.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Connector.h>

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
#include "play.h"
#include "player.h"

typedef ACE_Connector<OutputHandler, ACE_SOCK_Connector> Connector;
//Scene is composed by one or more Fragments. Each fragment owns particular players.
struct Fragment{
	Fragment(){};
	std::vector <std::shared_ptr<Part> > parts;
};

struct Scene{
	Scene(){}
	std::vector<std::shared_ptr<Fragment> > fragments;
};

class Director: public ACE_Svc_Handler<ACE_SOCK_Stream, ACE_NULL_SYNCH> {
public:
	Director(const std::string port, const std::string ip_address, unsigned int minimum_num_players_, const std::vector<std::string> scripts_);
	~Director();
	void cue(const std::string script_to_play, std::vector<std::string>& result);
	void stop(const std::string script_to_stop, std::vector<std::string>& result);
	void handler(const std::string command, std::vector<std::string>& result);
	int sendPlayList(Connector& connector);

	virtual int handle_input(ACE_HANDLE=ACE_INVALID_HANDLE);
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

