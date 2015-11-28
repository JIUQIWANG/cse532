#ifndef PLAY_LIST_H
#define PLAY_LIST_H

#include <ace/ACE.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Stream.h>
#include <list>
#include <map>
#include <string>
#include <iostream>
#include <memory>
#include <unordered_set>
typedef std::unordered_set<std::string> unique_set;

struct PlayItem{
    PlayItem(const std::string& name_, const int id_, const std::shared_ptr<ACE_SOCK_Stream>& stream_): name(name_), id(id_), stream(stream_), is_occupied(false), is_connected(true){}
    inline ACE_INET_Addr getAddr() const{
        ACE_INET_Addr addr;
        stream->get_remote_addr(addr);
    }
    std::string name;
    int id;
    const std::shared_ptr<ACE_SOCK_Stream> stream;
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
        ACE_INET_Addr addr = item.getAddr();
        addr.addr_to_string(buffer, 100);
        unique_addr.insert(std::string(buffer));
    }

    inline const std::list<PlayItem>& getList() const{
        return data;
    }
    inline bool is_empty() const{return data.empty();}

    void checkStatus();
    void maintainConnection(const ACE_INET_Addr& addr);
    std::string convertId(const std::string& id_str);

    bool find(const std::string& id_str, std::shared_ptr<ACE_SOCK_Stream>& stream) const;
private:
    std::list<PlayItem> data;
    unique_set unique_addr;
};


#endif
