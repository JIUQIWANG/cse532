#include "playlist.h"

using namespace std;

//remove the play item with certain address
void PlayList::removeAddr(const ACE_INET_Addr& target_addr){
    list<PlayItem>::iterator iter = data.begin();
    for(; iter!=data.end();){
	if(iter->addr == target_addr)
	    iter = data.erase(iter);
	else
	    ++iter;
    }
}

void PlayList::printList() const{
    list<PlayItem>::const_iterator iter = data.begin();
	cout << "Current PlayList" << endl;
	int id = 0;
	cout << "---------------------------" << endl;
    for(; iter!=data.end(); ++iter)
		cout << id++ <<' ' << iter->name << endl;
	cout << "---------------------------" << endl;

}