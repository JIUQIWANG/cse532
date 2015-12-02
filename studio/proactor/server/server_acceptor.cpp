#include "server_acceptor.h"
using namespace std;

void SocketHandler::open(ACE_HANDLE new_handle, ACE_Message_Block&){
	cout << "SocketHandler::open() called " << endl << std::flush;
	reader.open(*this, new_handle, 0, proactor());
	mblk.init(BUFSIZ);
	mblk.wr_ptr(mblk.base());
	reader.read(mblk, 1);
}

SocketHandler::~SocketHandler(){
	std::cout << "SocketHandler " << this << " destructed" << std::endl;
	reader.cancel();
	ACE_OS::closesocket(handle());
	acceptor->remove(this);
}

void SocketHandler::handle_read_stream(const ACE_Asynch_Read_Stream::Result& result){
	if(!result.success() || result.bytes_transferred() == 0)
		delete this;
	ACE_OS::write_n(ACE_STDOUT, mblk.rd_ptr(), result.bytes_transferred());
	mblk.wr_ptr(mblk.base());
	reader.read(mblk, 1);
}

SocketHandler* Server_Acceptor::make_handler(){
	SocketHandler* h;
	ACE_NEW_RETURN(h, SocketHandler(this), 0);
	if(client.insert(h) < 0){
		delete h;
		return 0;
	}
	return h;
}

void Server_Acceptor::close(){
	ACE_Unbounded_Set<SocketHandler*>::iterator iter = client.begin();
	SocketHandler** ih;
	while(iter.next(ih))
		delete *ih;
}