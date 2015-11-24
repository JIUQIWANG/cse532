#include "stdafx.h"
#include "director.h"
#include "string_util.h"

#define INVALID_LINES_NUM -1
#define SCRIPTS_START 0

using namespace std;

Director::Director(const std::string port, const std::string ip_address, unsigned int minimum_num_players_, const std::vector<std::string> scripts_) :scripts(scripts_), minimum_num_players(minimum_num_players_), max_players_consecutive(0), isOverride(false){
	//
	int position = SCRIPTS_START;
	while(position < (int)scripts.size()){
		storeScript(scripts[position], position);
		position++;
	}
	//construct player
	if (isOverride)
		num_players = minimum_num_players;
	else
		num_players = std::max(minimum_num_players, max_players_consecutive);
}

void Director::storeScript(const string script_name_, int position){
	ifstream script_file(script_name_.c_str());
	if (!script_file.is_open())
		throw runtime_error("Can not open script file!");

	string tempstring;
	unsigned int num_previous_part = INVALID_LINES_NUM;
	bool is_previous_config = false;
	int frag_counter = 0;
	vector<Scene> curScenes;
	vector<string> curSceneTitles;
	scene_titles.push_back(curSceneTitles);
	scenes.push_back(curScenes);
	//read script file and config files. While reading, fill the scenes member variable, which stores name and file path of each character
	while (getline(script_file, tempstring)){
		if (string_util::is_empty(tempstring))
			continue;
		vector<string> curline_split;
		string_util::split_str(tempstring, curline_split);
		if (curline_split[0] == "[scene]"){
			scenes[position].push_back(Scene());
			//compose the scene title
			string curtitle;
			for (size_t i = 1; i < curline_split.size() - 1; i++){
				curtitle.append(curline_split[i]);
				curtitle.append(" ");
			}
			curtitle.append(curline_split.back());
			scene_titles[position].push_back(curtitle);
			is_previous_config = false;
		}
		else{
			//store configuration files which are represented by Fragment in this lab.
			ifstream config_file(curline_split[0].c_str());
			if (!config_file.is_open()){
				cerr << "Can not open config file! " << curline_split[0] << endl;
				continue;
			}

			unsigned int cur_line_count = 0;
			shared_ptr <Fragment> curfrag(new Fragment());
			while (getline(config_file, tempstring)){
				vector<string> part_split;
				string_util::split_str(tempstring, part_split);
				if (part_split.size() != 2)
					continue;

				//The pair stores the information of configuration files' names and paths.
				shared_ptr<Part> part(new Part(frag_counter, pair<string, string>(part_split[0], part_split[1])));
				curfrag->parts.push_back(part);
				cur_line_count++;
			}
			config_file.close();
			scenes[position].back().fragments.push_back(curfrag);
			frag_counter++;

			//some helpful tricks to get correct form and results.
			if (num_previous_part == INVALID_LINES_NUM)
				num_previous_part = cur_line_count;
			else{
				max_players_consecutive = std::max(max_players_consecutive, num_previous_part + cur_line_count);
				num_previous_part = cur_line_count;
			}
			if (is_previous_config)
				scene_titles[position].push_back("");
			is_previous_config = true;
		}
	}
	script_file.close();
	plays.push_back(shared_ptr<Play>(new Play(scene_titles[position])));
}

Director::~Director(){
}

void Director::handler(const string command, vector<string>& result){
	vector<string> part_command;
	string_util::split_str(command, part_command);
	if (part_command[0] == "play"){
		cue(part_command[1], result);
	}
	else if (part_command[0] == "stop"){
		stop(part_command[1], result);
	}
}

void Director::stop(const string script_to_stop, vector<string>& result){
	auto to_play = find(scripts.begin(), scripts.end(), script_to_stop);
	if (to_play == scripts.end()){
		result.push_back("Can not find the script file, please check its name before running.");
		result.push_back("Usage: stop <path_to_script_file>");
		return;
	}
	int position = distance(scripts.begin(), to_play);
	plays[position]->stop();
	string tmp = "Stop " + script_to_stop + " finished!";
	players.clear();
	exceptionHandlers.clear();
	plays[position]->reset();
	result.push_back(tmp);
}

void Director::cue(const string script_to_play, vector<string>& result){
	int counter = 0;
	auto to_play = find(scripts.begin(), scripts.end(), script_to_play);
	if (to_play == scripts.end()){
		result.push_back("Can not find the script file, please check its name before running.");
		result.push_back("Usage: play <path_to_script_file>");
		return;
	}
	int position = distance(scripts.begin(), to_play);
	////////////////
	players.clear();
	exceptionHandlers.clear();
	////////////////
	for (unsigned int i = 0; i < num_players; i++){
		future<int> exceptionHandler;
		shared_ptr<Player> curplayer(new Player(*(plays[position])));
		curplayer->get_future(exceptionHandler);
		players.push_back(curplayer);
		exceptionHandlers.push_back(move(exceptionHandler));
	}
	cout << scene_titles[position][0] << endl;
	//enter all parts into players
	for (const auto s : scenes[position]){
		for (const auto f : s.fragments){
			for (const auto p : f->parts){
				players[counter % static_cast<int>(players.size())]->enter(p);
				counter++;
			}
		}
	}
	if (players.size() != exceptionHandlers.size())
		throw runtime_error("Exception: palyers.size() != exceptionHandlers.size()");
	//call exit function of each player and try to catch any exceptions
	for (size_t i = 0; i < players.size(); i++){
		players[i]->exit();
		try{
			exceptionHandlers[i].get();
		}
		catch (exception e){
			plays[position]->interrupt();
			cerr << e.what() << endl;
		}
	}
	string tmp = "Play " + script_to_play + " finished!";
	result.push_back(tmp); 
	plays[position]->reset();
}