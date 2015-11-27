#ifndef PLAY_LIST_H
#define PLAY_LIST_H

#include <ace/ACE.h>
#include <ace/INET_Addr.h>
#include <list>
#include <map>
#include <string>
#include <iostream>
struct PlayItem{
    PlayItem(const std::string& name_, const int id_, const ACE_INET_Addr& addr_): name(name_), id(id_), addr(addr_), is_occupied(false), is_connected(true){}
    std::string name;
    int id;
    ACE_INET_Addr addr;
    bool is_occupied;
    bool is_connected;
};
    
class PlayList{
public:
    void removeAddr(const ACE_INET_Addr& target_addr);
    void printList() const;
    inline void push_back(const PlayItem& item){
        data.push_back(item);
    }
    inline const std::list<PlayItem>& getList() const{
        return data;
    }

    std::string convertId(const std::string& id_str);

    bool find(const std::string& id_str, ACE_INET_Addr& addr) const;
private:
    std::list<PlayItem> data;
};


#endif
