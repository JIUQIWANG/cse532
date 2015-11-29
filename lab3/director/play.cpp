#include "play.h"
#include <chrono>
using namespace std;

//Recite the play based on the given Line and corresponding fragment number
void Play::recite(vector<Line>::iterator& iter, int fragment){
	unique_lock<mutex> guard(mt);
	//Skip when scene_fragment_counter > scene
	if (scene_fragment_counter == fragment && line_counter > iter->id){
		cerr << "Line missed (counter > line_id)!" << " scene counter: " << scene_fragment_counter << " line counter: " << line_counter << " line id: " << iter->id << endl << flush;
		iter++;
		cv.notify_all();
		return;
	}
	//Skip when counter > line.id
	if (scene_fragment_counter > fragment){
		cerr << "Fragment missed (counter > fragment)!" << scene_fragment_counter << ' ' << fragment << endl << flush;
		iter++;
		cv.notify_all();
		return;
	}


	//Use a condition variable to ensure the correct sequence.
	//Wait for at most 500 milliseconds, to avoid dead lock when some line numbers are missing
	//Also we add stop_flag.load() to ensure the recite can end sooner when receiving stop command from producer
	const auto max_duration = 500;
	if(cv.wait_for(guard, chrono::milliseconds(max_duration), [=]{return (scene_fragment_counter == fragment && line_counter == iter->id) || interrupt_flag.load(); })) {
		if(interrupt_flag.load()) {
			return;
		}
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

//Set interrupt_flag
void Play::interrupt(){
	interrupt_flag.store(true);
	enter_exit_cv.notify_all();
	cv.notify_all();
}

//Reset member variables for next time to play
void Play::reset() {
	line_counter = 1;
	scene_fragment_counter = 0;
	on_stage = 0;
	cur = start;
	cur++;
	interrupt_flag.store(false);
}

//Try to add one player in the specified play
Situation Play::enter(int enter_scenes){
	unique_lock<mutex> guard(mt);
	if (enter_scenes < scene_fragment_counter){
		return S_FAIL;
	}
	else if (enter_scenes == scene_fragment_counter){
		on_stage++;
		return S_SUCCESS;
	}
	else{
		enter_exit_cv.wait(guard,  [&]{return enter_scenes == this->scene_fragment_counter || interrupt_flag.load(); });
		if (interrupt_flag.load())
			return S_INTERRUPTED;

		on_stage++;
		return S_SUCCESS;
	}
}

//Try to exit one player in current play
Situation Play::exit(){
	unique_lock<mutex> guard(mt);
	if (on_stage > 1){
		on_stage--;
		return S_SUCCESS;
	}
	else if (on_stage < 1){
		return S_FAIL;
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
		return S_SUCCESS;
	}
}
