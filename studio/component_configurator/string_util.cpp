#include "string_util.h"

using namespace std;
namespace string_util{
	void split_str(const std::string& str, std::vector<std::string>& new_str){
		int start = 0;
		for (size_t i = 0; i < str.size(); i++){
			if (str[i] == ' ')
				continue;
			start = i;
			while (i < str.size() && str[i] != ' ')
				i++;
			string temp = str.substr(start, i - start);
			new_str.push_back(temp);
		}
	}

	bool is_empty(const std::string& str){
		if (str.size() == 0)
			return true;
		bool is_blank = true;
		for (size_t i = 0; i < str.size(); i++){
			if (str[i] != ' '){
				is_blank = false;
				break;
			}
		}
		return is_blank;
	}

	bool is_number(const string& str){
		for(const auto&v: str){
			if(v < '0' || v > '9')
				return false;
		}
		return true;
	}
} //namespace string_util