#ifndef __UDP_HANDLER__
#define __UDP_HANDLER__

#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <condition_variable>

const uint32_t kBUFF_SIZE = 1024;


class UDPReceiver{
private:
    int32_t fd_rx;
    int32_t optval;
    
    struct sockaddr_in clientadd;
    char buf[kBUFF_SIZE];

    std::thread recv_thread;
    
public:
    UDPReceiver(uint16_t port);

    int receive_monitor(uint32_t tid);
    int join_all();
    char* get_list();

};

#endif // __UDP_HANDLER__
