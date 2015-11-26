#ifndef STRING_UTIL_H
#define STRING_UTIL_H
#include <string>
#include <vector>

namespace string_util{
	//split str by a space character, and stored the splitted string into new_str
	void split_str(const std::string& str, std::vector<std::string>& new_str);

	//to judge whether a string is empty or consists only white spaces
	bool is_empty(const std::string& str);
}
#endif