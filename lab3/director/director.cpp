#include "director.h"
#include "play.h"
#include "player.h"

#define INVALID_LINES_NUM -1
#define SCRIPTS_START 0

using namespace std;

Director::Director(unsigned int minimum_num_players_, const std::vector<std::string> scripts_, const ACE_SOCK_Stream& stream_) :
	scripts(scripts_), minimum_num_players(minimum_num_players_), max_players_consecutive(0), isOverride(false), finish_token(-1), stream(stream_), local_port(-1){
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
		try {
			unique_lock<mutex> guard(mt);
			cv.wait(guard, [&] { return !this->play_queue.empty(); });
			int id = play_queue.front();
			if (id == finish_token)
				break;
			//the sendingMessage method will be called in both main thread and working thread, to avoid race condition, these two
			//calls are separated by the atomic<bool> variable inside SignalHandler. More specifically, working can only call
			//sendMessage before quit_flag is set, and the main thread can only call this method after quit_flag is set.
			if (SignalHandler::is_quit())
				break;
			string command = Protocal::composeCommand(Protocal::P_PLAYING, string(""), local_port);
			if (Sender::sendMessage(command, stream)) {
				cerr << "Connection lost" << endl;
				SignalHandler::set_quit_flag();
				break;
			}
			int sit = cue(id);
			if (sit == Situation::S_INTERRUPTED) {
				for (auto &v: players)
					v->interrupt();
			}
			play_queue.pop_front();
			//clean context
			for(auto& v: plays)
				v->reset();
			players.clear();
			exceptionHandlers.clear();
			guard.unlock();

			if (SignalHandler::is_quit())
				break;
			string feedback = Protocal::composeCommand(Protocal::P_FINISH, string(""), local_port);
			if (Sender::sendMessage(feedback, stream) < 0) {
				cerr << "Connection lost" << endl;
				SignalHandler::set_quit_flag();
				break;
			}
		}catch(const exception& e){
			cerr << "Director::work(): exception caught! Quit" << endl;
			play_queue.clear();
			exceptionHandlers.clear();
			SignalHandler::set_quit_flag();
			break;
		}
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
	//call exit() method to ensure the working thread won't block the deconstruction
	exit();
	try {
		workException.get();
	}catch(const runtime_error& e){
		cout << e.what() << endl;
	}
	if(workplay.joinable())
		workplay.join();
}

int Director::parseCommand(const string& command){
	string res;
	//Command is empty
	if(string_util::is_empty(command)){
		return Situation::S_FAIL;
	}
	Protocal::protocalType type;
	vector<string> arguments;
	unsigned short remote_port;
	if(Protocal::parseCommand(command, type, arguments, remote_port) < 0)
		return Situation::S_FAIL;

	if(type == Protocal::P_CHECK)
		return S_SUCCESS;

	//Command is quit
	if(type == Protocal::P_QUIT) {
		stop();
		SignalHandler::set_quit_flag();
		return Situation::S_SUCCESS;
	}

	int play_id = std::stoi(arguments.back());
	//The operated script ID is invalid
	if(play_id >= (int)scripts.size() || play_id < 0){
		return Situation::S_FAIL;
	}

	//Command is play
	if (type == Protocal::P_PLAY){
		submit(play_id);
	}
	else if (type == Protocal::P_STOP){
		stop();
	}
	return Situation::S_SUCCESS;
}

void Director::stop(){
	for(const auto& v: plays)
		v->interrupt();
}
int Director::cue(int id){
	int counter = 0;
	for (unsigned int i = 0; i < num_players; i++){
		future<int> exceptionHandler;
		shared_ptr<Player> curplayer(new Player(*(plays[id])));
		curplayer->get_future(exceptionHandler);
		players.push_back(curplayer);
		exceptionHandlers.push_back(move(exceptionHandler));
	}
	cout << scene_titles[id].front() << endl;

	//interruptable point
	if(plays[id]->is_interrupted())
		return Situation::S_INTERRUPTED;

	//Enter all parts into players.
	for (const auto s : scenes[id]){
		if(plays[id]->is_interrupted())
			return Situation::S_INTERRUPTED;
		for (const auto f : s.fragments){
			if(plays[id]->is_interrupted())
				return Situation::S_INTERRUPTED;
			for (const auto p : f->parts){
				if(plays[id]->is_interrupted())
					return Situation::S_INTERRUPTED;
				players[counter % static_cast<int>(players.size())]->enter(p);
				counter++;
			}
		}
	}
	if (players.size() != exceptionHandlers.size()){
		cerr << "Director::cue(): assertion failed: players.size() == exceptionHandlers.size()" << endl;
		return S_FAIL;
	}

	//Call exit function of each player and try to catch any exceptions
	for (size_t i = 0; i < players.size(); i++){
		//If the play is stopped, we will just break the loop
		if(plays[id]->is_interrupted())
			return Situation::S_INTERRUPTED;
		players[i]->exit();
		try{
			exceptionHandlers[i].get();
		}
		catch (const runtime_error& e){
			plays[id]->interrupt();
			cout << e.what() << endl;
			return S_INTERRUPTED;
		}
	}
	return S_SUCCESS;
}
