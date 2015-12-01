//
// Created by yanhang on 11/18/15.
//

#include "client.h"
#include "../socket_handler.h"
using namespace std;

PeriodSender::PeriodSender(int argc, char** argv):
      interval(3, 0), counter(0){
    messages.resize(argc - 1);
    for(int i=0; i<messages.size(); i++){
        messages[i] = string(argv[i+1]);
    }
    cout << "PeriodSender " << this << " constructed" << endl << std::flush;
}

int PeriodSender::open(void *) {
    if(reactor() &&
            reactor()->schedule_timer(this, 0, interval, interval) == -1)
        return -1;
    else return 0;
}

int PeriodSender::handle_timeout(const ACE_Time_Value &value, const void *pVoid) {
    printf("sending data...");
    char space = ' ';
    char numbuffer[10];

    ACE_SOCK_Stream& stream = peer();
    sprintf(numbuffer, "%d ", counter++);
    if (stream.send_n(numbuffer, strlen(numbuffer)) < 0) {
        cerr << endl << "Can not send message" << endl;
        return -1;
    }
    for(int i=0; i<messages.size(); i++) {
        if (stream.send_n(messages[i].c_str(), messages[i].size()) < 0) {
            cerr << endl << "Can not send message" << endl;
            return -1;
        }
        if (stream.send_n(&space,1) < 0) {
            cerr << endl << "Can not send message" << endl;
            return -1;
        }
    }
    printf("data sent\n");
    return 0;
}

int ClientConnector::connect(PeriodSender *&sh, const ACE_INET_Addr &remote_addr,
                             const ACE_Synch_Options &synch_options, const ACE_INET_Addr &local_addr, int reuse_addr,
                             int flags, int perms) {
    if(sh == 0){
        cerr << "Service handler must be constructed first" << endl;
        return -1;
    }
    unique_ptr<PeriodSender> guard_sender(sh);
    if(connect_svc_handler(sh, remote_addr, 0, local_addr, reuse_addr, flags, perms) < 0)
        return -1;
    if(activate_svc_handler(sh) < 0){
        return -1;
    }
    guard_sender.release();
    return 0;
}
