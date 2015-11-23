#include "playlist.h"

using namespace std;

//remove the play item with certain address
void PlayList::removeAddr(const ACE_INET_Addr& target_addr){
    list<PlayItem>::iterator iter = this->begin();
    for(; iter!=this->end();){
	if(iter->addr == target_addr)
	    iter = this->erase(iter);
	else
	    ++iter;
    }
}

void PlayList::printList() const{
    list<PlayItem>::const_iterator iter = this->begin();
    for(; iter!=this->end(); ++iter)
	cout << iter->name << endl;
}
