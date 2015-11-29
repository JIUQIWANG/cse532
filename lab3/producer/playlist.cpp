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
	system("cls");
	cout << "Current PlayList" << endl;
	int id = 0;
	cout << "---------------------------" << endl;
    for(const auto& v: data) {
	    cout << id++ << '\t' << v.name << "\t from:";
	    char addrbuffer[BUFSIZ];
	    v.getAddr().addr_to_string(addrbuffer, BUFSIZ);
		cout << addrbuffer << '\t';
		if(v.is_occupied)
			cout << "Playing" << endl;
		else
			cout << "Ready" << endl;
    }
	cout << "---------------------------" << endl;
}

int PlayList::find(const std::string &id_str, shared_ptr<ACE_SOCK_Stream>& stream)const {
	if(!is_number(id_str))
		return itemStatus::NOT_FOUND;
	int id = std::stoi(id_str);
	if(id >= (int)data.size())
		return itemStatus::NOT_FOUND;
	list<PlayItem>::const_iterator iter = data.begin();
	int i = 0;
	while(i++ < id)
		iter++;
	if(iter->is_occupied)
		return itemStatus::PLAYING;
	stream = iter->stream;
	return itemStatus::VALID;
}

string PlayList::convertId(const std::string &id_str) {
	if(!is_number(id_str))
		return string("");
	int id = stoi(id_str);
	if(id >= (int)data.size())
		return string("");
	list<PlayItem>::iterator iter = data.begin();
	int i=0;
	while(i++ < id)
		iter++;
	return to_string(iter->id);
}

bool PlayList::is_number(const string& str){
	for(const auto& v: str){
		if(v == windows_CR)
			continue;
		if(v < '0' || v > '9')
			return false;
	}
	return true;
}

void PlayList::occupy(const std::string& id_str, const ACE_INET_Addr& remote_addr){
	if(!is_number(id_str))
		return;
	int id = stoi(id_str);
	for(auto& v: data){
		if(v.getAddr() != remote_addr)
			continue;
		if(v.id == id)
			v.is_occupied = true;
	}
}

void PlayList::checkStatus() {
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
