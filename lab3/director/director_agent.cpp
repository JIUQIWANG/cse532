#include "director_agent.h"

using namespace std;

DirectorAgent::DirectorAgent(int argc, char **argv): timeout(0,100){
	int minimum_value = stoi(argv[argumentList::A_MINPLAYER]);
	vector<string> scripts;
	for (int pos = argumentList::A_SCRIPT; pos < argc; pos++) {
		scripts.push_back(argv[pos]);
	}
	director = shared_ptr<Director>(new Director(minimum_value, scripts, stream));

	//acceptor = shared_ptr<DirectorAcceptor>(new DirectorAcceptor(director));
	ACE_NEW_NORETURN(acceptor, DirectorAcceptor(director));
	acceptor_guard = unique_ptr<DirectorAcceptor>(acceptor);
	if(!acceptor){
		throw runtime_error("Can not allocate acceptor");
	}

	string addr_str = string(argv[argumentList::A_ADDRESS]);
	string port_str = string(argv[argumentList::A_PORT]);
	string full_str = addr_str+":"+port_str;
	if(addr_str.compare(dummy_address) == 0){
		full_str = "localhost:"+port_str;
		remote_addr.string_to_addr(full_str.c_str());
	}else {
		remote_addr.string_to_addr(full_str.c_str());
	}
	cout << "Producer address: "<<full_str<<endl;
}


DirectorAgent::~DirectorAgent(){
	acceptor->close();
	stream.close();
}

int DirectorAgent::open(char **argv){
	//open acceptor. The memory of acceptor will either be managed by reactor calling handle_close(),
	//or explicit released if initializeAcceptor() fails, thus the acceptor_guard should release the
	//ownership, to avoid re-release a released object.
	acceptor_guard.release();
	if(initializeAcceptor() < 0){
		cerr << "Can not open acceptor!" << endl;
		director->exit();
		delete acceptor;
		return returnType::E_CONNECTION;
	}
	cout << "Local acceptor opened, port:" << local_port << endl;
	director->set_localport(local_port);

	//connect to Producer
	if(connector.connect(stream, remote_addr) < 0){
		cerr << "Can not connect to Producer!" << endl;
		return returnType::E_CONNECTION;
	}
	return returnType::SUCCESS;
}

int DirectorAgent::run(){
	try {
		SignalHandler *sighandler;
		ACE_NEW_RETURN(sighandler, SignalHandler(), returnType::E_MEMORY);
		const ACE_Time_Value report_interval(1, 0);

		if (ACE_Reactor::instance()->register_handler(SIGINT, sighandler) < 0) {
			cerr << "DirectorAgent::run(): failed to register to reactor" << endl;
			//director->exit();
			return returnType::E_REACTOR;
		}

		if (sendPlayList() != Sender::SUCCESS) {
			cerr << "Can not connect to Producer" << endl;
			return returnType::E_CONNECTION;
		}

		while (true) {
			ACE_Reactor::instance()->handle_events(timeout);
			if (QuitFlags::is_quit()) {
				closeConnection();
				break;
			}
		}
	}catch(const exception& e){
		cerr << e.what() << endl;
		return returnType::E_OTHER;
	}
	return returnType::SUCCESS;
}

int DirectorAgent::sendPlayList(){
	string str = Protocal::composeCommand(Protocal::P_LIST, director->getPlayList(), local_port);
	return Sender::sendMessage(str, stream);
}

int DirectorAgent::initializeAcceptor(){
	//search for an avaliable port
	const unsigned short start_port = 3000;
	const unsigned short end_port = 10000;
	for(unsigned short i=start_port; i<=end_port; i++){
		if(acceptor->open(ACE_INET_Addr(i)) >= 0){
			local_port = i;
			return SUCCESS_RETURN;
		}
	}
	return ERROR_RETURN;
}

void DirectorAgent::closeConnection(){
	//after cleaning, send feedback to producer
	string str = Protocal::composeCommand(Protocal::P_QUIT, string(""), local_port);
	Sender::sendMessage(str, stream);
}