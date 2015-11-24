#include "play.h"
#include <chrono>
using namespace std;

//recite the play based on current counter.
void Play::recite(vector<Line>::iterator& iter, int fragment){
	unique_lock<mutex> guard(mt);
	//skip when scene_fragment_counter > scene
	if (stop_flag.load())
		return;
	if (scene_fragment_counter == fragment && line_counter > iter->id){
		cerr << "Line missed (counter > line_id)!" << " scene counter: " << scene_fragment_counter << " line counter: " << line_counter << " line id: " << iter->id << endl << flush;
		iter++;
		cv.notify_all();
		return;
	}
	//skip when counter > line.id
	if (scene_fragment_counter > fragment){
		cerr << "Fragment missed (counter > fragment)!" << scene_fragment_counter << ' ' << fragment << endl << flush;
		iter++;
		cv.notify_all();
		return;
	}


	//use a condition variable to ensure the correct sequence.
	//wait for at most 500 milliseconds, to avoid dead lock when some line numbers are missing
	const auto max_duration = 500;
	if(cv.wait_for(guard, chrono::milliseconds(max_duration), [=]{return scene_fragment_counter == fragment && line_counter == iter->id; })) {
		if (iter->character != current_character){
			cout << ' ' << endl;
			cout << iter->character << endl;
			current_character = iter->character;
		}
		cout << iter->text << endl;
		iter++;
	}
	line_counter++;
	cv.notify_all();
}

//set interrupt_flag
void Play::interrupt(){
	interrupt_flag.store(true);
	enter_exit_cv.notify_all();
}

void Play::stop(){
	stop_flag.store(true);
}

void Play::reset() {
	line_counter = 1;
	scene_fragment_counter = 0;
	on_stage = 0;
	cur = start;
	cur++;
	stop_flag.store(false);
}


//try to add one player in the specified play
Situation Play::enter(int enter_scenes){
	unique_lock<mutex> guard(mt);
	if (enter_scenes < scene_fragment_counter){
		return failEnter;
	}
	else if (enter_scenes == scene_fragment_counter){
		on_stage++;
		return successEnter;
	}
	else{
		//wait for a max time, to avoid failing entering blocking the whole process. This duration should be much larger than the one in recite
		enter_exit_cv.wait(guard,  [&]{return enter_scenes == this->scene_fragment_counter || interrupt_flag.load(); });
		if (interrupt_flag.load())
			return interrupted;

		on_stage++;
		return successEnter;
	}
}

//try to exit one player in current play
Situation Play::exit(){
	unique_lock<mutex> guard(mt);
	if (on_stage > 1){
		on_stage--;
		return successExit;
	}
	else if (on_stage < 1){
		return failExit;
	}
	else{
		on_stage--;
		scene_fragment_counter++;
		if (cur != end){
			std::cout << std::endl;
			std::cout << (*cur) << std::endl;
			cur++;
		}
		line_counter = 1;
		current_character = " ";
		enter_exit_cv.notify_all();
		return successExit;
	}
}
