#include "playlist.h"

using namespace std;


//remove the play item with certain address
void PlayList::removeAddr(const ACE_INET_Addr& target_addr) {
	list<ItemSet>::iterator iter = data.begin();
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
	if(is_empty()){
		cout << "No director connected" << endl;
		return;
	}
	int id = 0;
	cout << "---------------------------" << endl;
    for(const auto& v: data) {
	    char addrbuffer[BUFSIZ];
	    v.getAddr().addr_to_string(addrbuffer, BUFSIZ);
	    for (const auto&i: v.item){
		    cout << id++ << '\t' << i.name << "\t from:";
		    cout << addrbuffer << '\t';
		    if (v.is_occupied)
			    cout << "Playing" << endl;
		    else
			    cout << "Ready" << endl;
	    }
    }
	cout << "---------------------------" << endl;
}

void PlayList::printAddress() const{
	if(is_empty()){
		cout << "No director connected" << endl;
		return;
	}
	for(const auto& v:data){
		char addrbuffer[BUFSIZ];
		v.getAddr().addr_to_string(addrbuffer, BUFSIZ);
		cout << addrbuffer << endl << flush;
	}
}
int PlayList::find(const std::string &id_str, shared_ptr<ACE_SOCK_Stream>& stream)const {
	if(!is_number(id_str))
		return itemStatus::NOT_FOUND;
	int id = std::stoi(id_str);

	int counter = 0;
	for(const auto& v: data){
		for(const auto& vv: v.item){
			if(counter++ == id){
				stream = v.stream;
				if(v.is_occupied)
					return itemStatus::PLAYING;
				return itemStatus::VALID;
			}
		}
	}
	return itemStatus::NOT_FOUND;
}

string PlayList::convertId(const std::string &id_str) {
	if(!is_number(id_str))
		return string("");
	int id = stoi(id_str);
	int counter = 0;
	for(const auto& v: data){
		for(const auto& vv: v.item){
			if(counter++ == id){
				return to_string(vv.id);
			}
		}
	}
	return string("");
}

bool PlayList::is_number(const string& str){
	for(const auto& v: str){
		if(v < '0' || v > '9')
			return false;
	}
	return true;
}

void PlayList::occupy(const ACE_INET_Addr& remote_addr){
	char buffer[BUFSIZ];
	remote_addr.addr_to_string(buffer, BUFSIZ);
	cout << "remote address: " << buffer << endl;
	for(auto& v: data){
		v.getAddr().addr_to_string(buffer, BUFSIZ);
		cout << buffer << endl;
		if(v.getAddr() == remote_addr) {
			cout << "c1" << endl;
			v.is_occupied = true;
		}
	}
}

void PlayList::release(const ACE_INET_Addr &remote_addr) {
	for(auto& v: data){
		if(v.getAddr() == remote_addr)
			v.is_occupied = false;
	}
}
