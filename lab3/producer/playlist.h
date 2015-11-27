#ifndef PLAY_LIST_H
#define PLAY_LIST_H

#include <ace/ACE.h>
#include <ace/INET_Addr.h>
#include <list>
#include <map>
#include <string>
#include <iostream>
#include <unordered_set>
typedef std::unordered_set<std::string> unique_set;

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
        char buffer[100] = {};
        item.addr.addr_to_string(buffer, 100);
        unique_addr.insert(std::string(buffer));
    }

    inline const std::list<PlayItem>& getList() const{
        return data;
    }
    inline bool is_empty() const{return data.empty();}

    void checkStatus();
    void maintainConnection(const ACE_INET_Addr& addr);
    std::string convertId(const std::string& id_str);

    bool find(const std::string& id_str, ACE_INET_Addr& addr) const;
private:
    std::list<PlayItem> data;
    unique_set unique_addr;
};


#endif
