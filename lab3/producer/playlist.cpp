#include "playlist.h"

using namespace std;

//remove the play item with certain address
void PlayList::removeAddr(const ACE_INET_Addr& target_addr) {
	list<PlayItem>::iterator iter = data.begin();
	int counter = 0;
	for (; iter != data.end();) {
		if (iter->addr == target_addr) {
			iter = data.erase(iter);
		} else {
			++iter;
		}
	}
}

void PlayList::printList() const{
    list<PlayItem>::const_iterator iter = data.begin();
	cout << "Current PlayList" << endl;
	int id = 0;
	cout << "---------------------------" << endl;
    for(; iter!=data.end(); ++iter) {
	    cout << id++ << '\t' << iter->name << "\t from:";
	    char addrbuffer[1024];
	    iter->addr.addr_to_string(addrbuffer, 1024);
		cout << addrbuffer << endl;
    }
	cout << "---------------------------" << endl;

}

bool PlayList::find(const std::string &id_str, ACE_INET_Addr& addr)const {
	for(const auto& v: id_str){
		if(v < '0' || v > '9')
			return false;
	}
	int id = std::stoi(id_str);
	if(id >= data.size())
		return false;
	list<PlayItem>::const_iterator iter = data.begin();
	int i = 0;
	while(i++ < id)
		iter++;
	addr = iter->addr;
	return true;
}

string PlayList::convertId(const std::string &id_str) {
	for(const auto& v: id_str){
		if(v < '0' || v > '9'){
			cerr << "PlayList::convertId(): invalid id_str" << endl;
			return string("");
		}
	}
	int id = stoi(id_str);
	if(id >= data.size())
		return string("");
	list<PlayItem>::iterator iter = data.begin();
	int i=0;
	while(i++ < id)
		iter++;
	return to_string(iter->id);
}