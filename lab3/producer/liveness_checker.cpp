//
// Created by Yan Hang on 11/26/15.
//

#include "liveness_checker.h"

using namespace std;
int LivenessChecker::handle_timeout(const ACE_Time_Value& value, const void *pvoid){
	playlist->checkStatus();
	return 0;
}