#ifndef __UDP_INTF__
#define __UDP_INTF__
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <string>
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

#include "client_list.h"
#include "../util/payload_util.h"


// take server/client port as sys arg

#define BUFSIZE         1024
#define MAX_THREADS      100
// TODO: hard code register sever info.

class UDPIntf{
private:    
    int32_t fd_rx;
    int32_t fd_tx;
    int16_t portno;
    int32_t optval;
    uint32_t registry_ip;
    uint16_t registry_port;

    struct sockaddr_in serveradd;
    struct sockaddr_in clientadd;
    char buf[BUFSIZE];



    std::thread thread_pool[MAX_THREADS];

    // 
    ClientList *_cli_obj;
    PayLoad *_pl_obj;

public:
    UDPIntf(uint16_t port, uint32_t registry_ip, uint16_t registry_port, 
            void *_client_list_obj, void *payload_obj);
    std::queue<struct article> bcast_msg;
    int join_all();
    std::thread recv_thread;
    std::thread bcast_monitor;
   
    std::mutex bcast_lck;
    std::condition_variable cv;
    bool bcast_signal;

    int register_server();
    int deregister();

    int send_article(struct sockaddr *clientadd, char *article, size_t size);
    int send_to_clients(uint32_t ip, uint16_t port, char *msg);


    int receive_monitor(uint32_t tid);
    // don't think we need a separate thread here, but upto you
    int sender_monitor(uint32_t tid);

    // 
    int send_or_die(std::vector<struct article> *subscriptions, 
                    struct node *client,
                    struct article msg, uint32_t tid);

    int acquire_bcast_lock();
    int acquire_bcast_unlock();

    void set_bcast_signal(bool val);
    void notify();
    int get_port_number();
    void add_pub_msg(struct article *msg); 
    
    uint32_t get_registry_ip();
    uint16_t get_registry_port();

    
};


#endif // __UDP_INTF__
