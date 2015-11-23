#ifndef PLAY_LIST_H
#define PLAY_LIST_H

#include <ace/ACE.h>
#include <ace/INET_Addr.h>
#include <list>
#include <string>
#include <iostream>
struct PlayItem{
    std::string name;
    ACE_INET_Addr addr;
};
    
class PlayList: public std::list<PlayItem>{
public:
    void removeAddr(const ACE_INET_Addr& target_addr);
    void printList() const;
};


#endif
