#include "common.h"
using namespace std;

int Protocal::parseCommand(const std::string& str, protocalType& type, std::vector<std::string>& arguments, unsigned short& remote_port){
	std::vector<std::string> str_split;
	string_util::split_str(str, str_split);

	//extract remote port number
	for(const auto& v: str_split.back()){
		if(v < '0' || v > '9'){
			cerr << "Protocal::parseCommand(): invalid port number: " << str_split.back() << endl;
			return -1;
		}
	}
	remote_port = (unsigned short)std::stoi(str_split.back());

	std::string mid;
	//compose the substring containing all play titles
	for(size_t i=1; i<str_split.size()-1; ++i)
		mid.append(str_split[i]);

	if(str_split.front().compare("<list>") == 0){
		type = P_LIST;
		int startid = -1;
		for(int i=0; i<(int)mid.size(); i++)
			if(mid[i] == '('){
				startid = i + 1;
			}else if(mid[i] == ')') {
				if (startid == -1) {
					cerr << "Protocal::parseCommand(): Mismatched parentheses!" << endl;
					return -1;
				}
				arguments.push_back(mid.substr(startid, i - startid));
				startid = -1;
			}
		if(startid != -1){
			cerr << "Protocal::parseCommand(): Mismatched parentheses!" << endl;
			return -1;
		}
	}else if(str_split.front().compare("<quit>") == 0){
		type = P_QUIT;
	}else if(str_split.front().compare("<play>") == 0){
		type = P_PLAY;
		arguments.push_back(mid);
	}else if(str_split.front().compare("<playing>") == 0){
		type = P_PLAYING;
		arguments.push_back(mid);
	}else if(str_split.front().compare("<stop>") == 0) {
		type = P_STOP;
		arguments.push_back(mid);
	}else if(str_split.front().compare("<finish>") == 0) {
		type = P_FINISH;
		arguments.push_back(mid);
	}else if(str_split.front().compare("<check>") == 0){
		type = P_CHECK;
		arguments.push_back(mid);
	}else{
		cout << "Protocal::parseCommand(): protocal error, omit command: " << str_split[0] << endl;
		return -1;
	}
	return 0;
}

std::string Protocal::composeCommand(const protocalType& type, const std::vector<std::string>& arguments, const unsigned short local_port){
	std::string str;
	switch(type){
		case P_LIST:
			str.append("<list> ");
			break;
		case P_PLAY:
			str.append("<play> ");
			break;
		case P_PLAYING:
			str.append("<playing> ");
			break;
		case P_FINISH:
			str.append("<finish>");
			break;
		case P_STOP:
			str.append("<stop> ");
			break;
		case P_QUIT:
			str.append("<quit> ");
			break;
		case P_CHECK:
			str.append("<check>");
			break;
		default:
			cerr << "Protocal::composeCommand(): invalid command type!" << endl;
			str = "<invalid> " + std::to_string(local_port);
			return str;
	}
	for(const auto& v: arguments)
		str.append(v + " ");
	str.append(std::to_string(local_port));
	return str;
}

std::string Protocal::composeCommand(const protocalType& type, const std::string& str, const unsigned short local_port){
	std::vector<std::string> arguments;
	arguments.push_back(str);
	return composeCommand(type, arguments, local_port);
}
