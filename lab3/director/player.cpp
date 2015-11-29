#include "player.h"
#include "../string_util.h"
using namespace std;

//Constructor of Player class. Create and launch the working thread, get the future of it
Player::Player(Play& play_) :play(play_){
	packaged_task<int()> task(bind(&Player::read, this));
	exceptionHandler = task.get_future();
	t = thread(move(task));
}


Player::~Player() {
	if (t.joinable())
		t.join();
}

//Build Lines for Player object and start act() function.
int Player::read(){
	while (true){
		unique_lock<mutex> guard(mt);
		cv.wait(guard, [&]{return !this->working_queue.empty();});

		shared_ptr<Part> part = working_queue.front();
		working_queue.pop_front();
		if(part->fragmentid == TERMINATION_TOKEN) {
			working_queue.clear();
			break;
		}
		int sit = actPart(part);
		if(sit != Situation::S_SUCCESS)
			break;
	}
	return 0;
}

//Act specified part
int Player::actPart(const std::shared_ptr<Part> &part){
	lines.clear();
	Situation entersit = play.enter(part->fragmentid);
	if (entersit == Situation::S_FAIL){
		throw runtime_error("player " + part->str.first + " enter failed");
	}else if(entersit == Situation::S_INTERRUPTED)
		return Situation::S_INTERRUPTED;

	string tempstr;
	ifstream fin(part->str.second.c_str());
	if(!fin.is_open()){
		cerr << "Can not open file "<< part->str.second<<endl << flush;
		return Situation::S_FAIL;
	}
	while (std::getline(fin, tempstr)){
		Line templine;
		//Parse input string into Line objects.
		parseString(tempstr, part->str.first, templine);
		if (templine.id > 0){ lines.push_back(templine); }
	}
	//Sort lines w.r.t. line id, to handle unordered lines.
	sort(lines.begin(), lines.end(), [](const Line& l1, const Line& l2){return l1.id < l2.id; });
	act(part->fragmentid);

	//After acting, exit the stage
	Situation exitsit = play.exit();
	if (exitsit == Situation::S_FAIL){
		throw runtime_error("Player " + part->str.second + " failed to exit!");
	}else if(exitsit == Situation::S_INTERRUPTED)
		return Situation::S_INTERRUPTED;
	return Situation::S_SUCCESS;
}

//call recite() function of Play
void Player::act(int fragmentid){
	vector<Line>::iterator iter = lines.begin();
	while (iter != lines.end() && !play.is_interrupted()){
		play.recite(iter, fragmentid);
	}
}

//start thread by move a new thread.
void Player::enter(const shared_ptr<Part> part){
	lock_guard<mutex> guard(mt);
	working_queue.push_back(part);
	cv.notify_all();
}

void Player::exit(){
	//Add termination token into working queue
	shared_ptr<Part> terminal_part(new Part(TERMINATION_TOKEN,pair<string,string>()));
	enter(terminal_part);
}

void Player::interrupt() {
	shared_ptr<Part> interrupt_part(new Part(TERMINATION_TOKEN,pair<string,string>()));
	lock_guard<mutex> guard(mt);
	working_queue.push_front(interrupt_part);
	cv.notify_all();
}

void Player::parseString(const string& str, const string& character, Line& line){
	if (string_util::is_empty(str))
		return;
	vector<string> newstr;
	string_util::split_str(str, newstr);

	//Check if the first string is a number
	for (const auto& c : newstr[0]){
		if (c < '0' || c > '9')
			return;
	}

	line.id = stoi(newstr[0]);
	line.character = character;

	line.text = "";
	for (size_t i = 1; i < newstr.size(); i++)
		line.text = line.text + ' ' + newstr[i];
}
