#include "playlist.h"

using namespace std;

const char PlayList::windows_CR = 13;

//remove the play item with certain address
void PlayList::removeAddr(const ACE_INET_Addr& target_addr) {
	list<PlayItem>::iterator iter = data.begin();
	int counter = 0;
	for (; iter != data.end();) {
		if (iter->getAddr() == target_addr) {
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
	    char addrbuffer[BUFSIZ];
	    iter->getAddr().addr_to_string(addrbuffer, BUFSIZ);
		cout << addrbuffer << endl;
    }
	cout << "---------------------------" << endl;

}

bool PlayList::find(const std::string &id_str, shared_ptr<ACE_SOCK_Stream>& stream)const {
	for(const auto& v: id_str){
		if(v == windows_CR)
			continue;
		if(v < '0' || v > '9'){
			return false;
		}
	}
	int id = std::stoi(id_str);
	if(id >= data.size())
		return false;
	list<PlayItem>::const_iterator iter = data.begin();
	int i = 0;
	while(i++ < id)
		iter++;
	stream = iter->stream;
	return true;
}

string PlayList::convertId(const std::string &id_str) {
	for(const auto& v: id_str){
		if(v == windows_CR)
			continue;
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

void PlayList::checkStatus()q {
	unique_set addr_to_remove;
	bool is_updated = false;
	for(auto& v: data){
		if(v.is_connected) {
			v.is_connected = false;
			continue;
		}
		is_updated = true;
		char buffer[BUFSIZ];
		v.getAddr().addr_to_string(buffer, BUFSIZ);
		addr_to_remove.insert(string(buffer));
	}

	for(auto&v: addr_to_remove){
		cout << v << " disconnected." << endl;
		ACE_INET_Addr addr;
		addr.string_to_addr(v.c_str());
		removeAddr(addr);
	}

	if(is_updated)
		printList();
}

void PlayList::maintainConnection(const ACE_INET_Addr &addr) {
	for(auto& v: data){
		if(v.getAddr() == addr)
			v.is_connected = true;
	}
}