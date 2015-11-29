#include "director.h"
#include "play.h"
#include "player.h"

#define INVALID_LINES_NUM -1
#define SCRIPTS_START 0

using namespace std;

Director::Director(unsigned int minimum_num_players_, const std::vector<std::string> scripts_) :
	scripts(scripts_), minimum_num_players(minimum_num_players_), max_players_consecutive(0), isOverride(false), finish_token(-1){
	//Add mutiple scripts
	int position = SCRIPTS_START;
	while(position < (int)scripts.size()){
		storeScript(scripts[position], position);
		position++;
	}
	//Compute num_players
	if (isOverride)
		num_players = minimum_num_players;
	else
		num_players = max(minimum_num_players, max_players_consecutive);

	//Make Director Active Object with a thread to continue work
	packaged_task<int()> task(bind(&Director::work, this));
	workException = task.get_future();
	workplay = thread(move(task));
}

int Director::work(){
	//Continue pull script ID and call cue(id) to play the specific script
	while(true){
		unique_lock<mutex> guard(mt);
		cv.wait(guard, [&]{return !this->play_queue.empty();});
		int id = play_queue.front();
		if(id == finish_token)
			break;
		cue(id);
		play_queue.pop_front();
	}
	return 0;
}

//Return formatted play list
string Director::getPlayList()const {
	string full_list;
	for(const auto& v: scripts)
		full_list = full_list + '(' + v + ')';
	return full_list;
}

//Store the specific script
void Director::storeScript(const string script_name_, int position){
	ifstream script_file(script_name_.c_str());
	//Open the script file
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
	//Read script file and config files. While reading, fill the scenes member variable, which stores name and file path of each character
	while (getline(script_file, tempstring)){
		if (string_util::is_empty(tempstring))
			continue;
		vector<string> curline_split;
		string_util::split_str(tempstring, curline_split);
		if (curline_split[0] == "[scene]"){
			scenes[position].push_back(Scene());
			//Compose the scene title
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
			//Store configuration files which are represented by Fragment in this lab.
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
				max_players_consecutive = max(max_players_consecutive, num_previous_part + cur_line_count);
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
	if(workplay.joinable())
		workplay.join();
}

string Director::parseCommand(const string& command){
	string res;
	//Command is empty
	
	return res;
}

string Director::stop(int id){
	string res;
	//If the specific play is not running at all
	if(!plays[id]->isWorking())
		res = "Play " + to_string(id) + " is not running";
	//Otherwise we will stop the specific play
	else {
		plays[id]->stop();
		//In case of race condition between stop() and cue(), we add a mutex here
		unique_lock<mutex> guard(mu);
		players.clear();
		exceptionHandlers.clear();
		guard.unlock();
		//After the stop, we still need to reset the specific play member variables
		plays[id]->reset();
		res = "Stop Successfully";
	}
	return res;
}

void Director::cue(int id){
	int counter = 0;
	string res;

	//Each time we start playing a new script, players and excpetionHandlers need to be reset
	players.clear();
	exceptionHandlers.clear();

	for (unsigned int i = 0; i < num_players; i++){
		future<int> exceptionHandler;
		shared_ptr<Player> curplayer(new Player(*(plays[id])));
		curplayer->get_future(exceptionHandler);
		players.push_back(curplayer);
		exceptionHandlers.push_back(move(exceptionHandler));
	}
	cout << scene_titles[id][0] << endl;
	//Enter all parts into players.
	//Here we add a mutex to ensure there will not be race condition when calling stop() function which clear players and exceptionHandlers
	unique_lock<mutex> guard(mu);
	for (const auto s : scenes[id]){
		for (const auto f : s.fragments){
			for (const auto p : f->parts){
				players[counter % static_cast<int>(players.size())]->enter(p);
				counter++;
			}
		}
	}
	if (players.size() != exceptionHandlers.size())
		throw runtime_error("Exception: palyers.size() != exceptionHandlers.size()");
	//Call exit function of each player and try to catch any exceptions
	for (size_t i = 0; i < players.size(); i++){
		//If the play is stopped, we will just break the loop
		if(plays[id]->isStop())
			break;
		players[i]->exit();
		try{
			exceptionHandlers[i].get();
		}
		catch (exception e){
			plays[id]->interrupt();
			cerr << e.what() << endl;
		}
	}
	//Produce corresponding results based on whether or not we stop the play
	if(plays[id]->isStop())
		res = "Play Interrupted";
	else {
		res = "Play Successfully";
		plays[id]->reset();
	}
	guard.unlock();
	cout << res << endl;
}
