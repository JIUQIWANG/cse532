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
    PlayItem(const std::string& name_, const int id_): name(name_), id(id_){}
    std::string name;
    int id;
};

struct ItemSet{
    ItemSet(const std::shared_ptr<ACE_SOCK_Stream> stream_): stream(stream_), item(), is_occupied(false), is_connected(true){}
    const std::shared_ptr<ACE_SOCK_Stream> stream;
    inline ACE_INET_Addr getAddr() const{
        ACE_INET_Addr addr;
        stream->get_remote_addr(addr);
        return addr;
    }
    std::list<PlayItem> item;
    bool is_occupied;
    bool is_connected;
};

class PlayList{
public:
	enum itemStatus{VALID, NOT_FOUND, PLAYING};
    void removeAddr(const ACE_INET_Addr& target_addr);
    void printList() const;
    inline void push_back(const ItemSet& item){
        data.push_back(item);
    }

    inline const std::list<ItemSet>& getList() const{
        return data;
    }
    inline bool is_empty() const{return data.empty();}

    void checkStatus();
    void maintainConnection(const ACE_INET_Addr& addr);
    std::string convertId(const std::string& id_str);

	static bool is_number(const std::string& str);

    int find(const std::string& id_str, std::shared_ptr<ACE_SOCK_Stream>& stream) const;
	void occupy(const ACE_INET_Addr& remote_addr);
    void release(const ACE_INET_Addr& remote_addr);
private:
    std::list<ItemSet> data;
	static const char windows_CR;
};


#endif
