#include "player.h"
#include "string_util.h"
using namespace std;

//constructor of Player class. Create and launch the working thread, get the future of it
Player::Player(Play& play_) :play(play_){
	packaged_task<int()> task(bind(&Player::read, this));
	exceptionHandler = task.get_future();
	t = thread(move(task));
}


Player::~Player() {
	//exit();
	if (t.joinable())
		t.join();
}

//build Lines for Player object and start act() function.
int Player::read(){
	while (true){
		unique_lock<mutex> guard(mt);
		cv.wait(guard, [&]{return !this->working_queue.empty();});

		shared_ptr<Part> part = working_queue.front();
		working_queue.pop_front();
		if(part->fragmentid == TERMINATION_TOKEN)
			break;
		actPart(part);
	}
	return 0;
}

//act specified part
void Player::actPart(const std::shared_ptr<Part> &part){
	lines.clear();
	Situation entersit = play.enter(part->fragmentid);
	if (entersit != Situation::successEnter){
		throw runtime_error("player " + part->str.first + " enter failed");
	}

	string tempstr;
	ifstream fin(part->str.second.c_str());
	if(!fin.is_open()){
		cerr << "Can not open file "<< part->str.second<<endl << flush;
		return;
	}
	while (std::getline(fin, tempstr)){
		Line templine;
		//parse input string into Line objects.
		parseString(tempstr, part->str.first, templine);
		if (templine.id > 0){ lines.push_back(templine); }
	}
	//Sort lines w.r.t. line id, to handle unordered lines.
	sort(lines.begin(), lines.end(), [](const Line& l1, const Line& l2){return l1.id < l2.id; });
	act(part->fragmentid);

	//after acting, exit the stage
	Situation exitsit = play.exit();
	if (exitsit != successExit){
		cerr << "Player " <<part->str.second << " failed to exit!" << endl << flush;
		throw runtime_error("player exit failed");
	}
}

//call recite() function of Play
void Player::act(int fragmentid){
	vector<Line>::iterator iter = lines.begin();
	while (iter != lines.end()){
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
	Part terminal_part(TERMINATION_TOKEN,pair<string,string>());
	enter(make_shared<Part>(terminal_part));
}

void Player::parseString(const string& str, const string& character, Line& line){
	if (string_util::is_empty(str))
		return;
	vector<string> newstr;
	string_util::split_str(str, newstr);

	//check if the first string is a number
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