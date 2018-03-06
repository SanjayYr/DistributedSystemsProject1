#include "udp_intf.h"
#include "../util/common_util.h"
#include "../rpc_gen.h"

#define UDP_BUFF_SIZE 2048
// #define REG_SERVER_IP /*"128.101.37.9"*/ "128.101.35.147"
#define REG_SERVER_IP /*"128.101.37.9"*/ "128.101.37.3"
#define REG_SERVER_PORT 5104

UDPIntf::UDPIntf(uint16_t port, uint32_t registry_ip, uint16_t registry_port, 
                 void *client_list_obj, 
                 void *payload_obj){
    portno = port; 
    this->registry_ip = registry_ip;
    this->registry_port = registry_port;

    _cli_obj = static_cast<ClientList*>(client_list_obj);
    _pl_obj = static_cast<PayLoad*>(payload_obj);
    fd_rx = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd_rx < 0){
        perror("OS did not give an fd for UDP rx socket");
    }
    std::cout << "RX FD : " << fd_rx << std::endl;
    fd_tx = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd_tx < 0){
        perror("OS did not give an fd for UDP tx socket");
    }

    optval = 1;
    setsockopt(fd_rx, SOL_SOCKET, SO_REUSEADDR, 
               (const void *)&optval , sizeof(int));

    setsockopt(fd_tx, SOL_SOCKET, SO_REUSEADDR, 
               (const void *)&optval , sizeof(int));

    serveradd.sin_family = AF_INET;
    serveradd.sin_addr.s_addr = htonl(INADDR_ANY);
    serveradd.sin_port = htons((unsigned short)port);

    if (bind(fd_rx, (struct sockaddr *) &serveradd, 
        sizeof(serveradd)) < 0){
        perror("ERROR on binding");
    }

  /*  if (this->register_server() < 0){
        perror("Could not register to the main server");
    }*/
    std::cout << "here" << std::endl;
    bcast_signal = false;
    this->recv_thread = std::thread(&UDPIntf::receive_monitor, this, 99);
    this->bcast_monitor = std::thread(&UDPIntf::sender_monitor, this, 98);

}

int UDPIntf::register_server(){
    std::cout << "Register RPCs to registry server";
    
    char *serverIp = CommonUtil::getMyIpAddress(); 
    int port  = get_port_number();
    int port_reg  = get_registry_port();

    std::string send = "";
    std::string prefix = "";
    
    prefix += std::string("Register") + ";" + std::string("RPC") + ";" + serverIp + ";" + std::to_string(port) + ";";
    send += prefix + std::to_string(Join) + ";" + std::to_string(COMMUNICATE_VERSION);
    
    send_to_clients(get_registry_ip(), htons(port_reg), strdup(send.c_str())); 
    send = "";

    send += prefix + std::to_string(Leave) + ";" + std::to_string(COMMUNICATE_VERSION);
    send_to_clients(get_registry_ip(), htons(port_reg), strdup(send.c_str())); 
    send = "";

    send += prefix + std::to_string(Subscribe) + ";" + std::to_string(COMMUNICATE_VERSION);
    send_to_clients(get_registry_ip(), htons(port_reg), strdup(send.c_str()));
    send = "";

    send += prefix + std::to_string(Unsubscribe) + ";" + std::to_string(COMMUNICATE_VERSION);
    send_to_clients(get_registry_ip(), htons(port_reg), strdup(send.c_str())); 
    send = "";

    send += prefix + std::to_string(Publish) + ";" + std::to_string(COMMUNICATE_VERSION);
    send_to_clients(get_registry_ip(), htons(port_reg),strdup(send.c_str())); 
    send = "";

    send += prefix + std::to_string(PublishServer) + ";" + std::to_string(COMMUNICATE_VERSION);
    send_to_clients(get_registry_ip(), htons(port_reg),  strdup(send.c_str())); 
    send = "";
    
    send += prefix + std::to_string(Ping) + ";" + std::to_string(COMMUNICATE_VERSION);
    send_to_clients(get_registry_ip(), htons(port_reg), strdup(send.c_str())); 
    send = "";

 
    return 1; 
}

int UDPIntf::join_all(){
    this->recv_thread.join();
}

int UDPIntf::send_to_clients(uint32_t ip, uint16_t port, char *msg){
    memset((char *) &clientadd, 0, sizeof(clientadd));
    clientadd.sin_family = AF_INET;
    clientadd.sin_addr.s_addr = ip;
    //inet_aton("10.0.0.47", &clientadd.sin_addr);
    std::cout << "Port: " << port << std::endl;
    clientadd.sin_port = (port);
    int addrlen = sizeof(clientadd);
    char buf_x[120];
    sprintf(buf_x, "%s", msg);
    std::cout << "sending packet: " << msg 
              << " addr_len: " << addrlen << std::endl;
    if(sendto(this->fd_rx, buf_x, strlen(buf_x), 0, 
                (struct sockaddr *)&clientadd, addrlen)<0){
        perror("Unable to send");
    }   
    // TODO: remove logging
    std::cout << "sent packet: " << msg 
              << " addr_len: " << addrlen << std::endl;
    fflush(stdout);
}

int UDPIntf::receive_monitor(uint32_t tid){
    std::cout << "Receiver tid: "<< tid << std::endl;
    char buff[2048];
    int len;
    struct sockaddr_in recvadd;
    memset((char *)&recvadd, 0, sizeof(recvadd));
    recvadd.sin_addr.s_addr = htonl(INADDR_ANY);
    recvadd.sin_port = htons(get_port_number());
    socklen_t socklen = sizeof(recvadd);
    if (bind(fd_tx, (struct sockaddr *)&recvadd, socklen) < 0) {
        perror("bind failed");
    }
    while(1){
        len = recvfrom(this->fd_tx, buff, UDP_BUFF_SIZE, 0, 
                        (struct sockaddr *)&recvadd, &socklen);
        if (len > 0){
            if(sendto(this->fd_tx, buff, strlen(buff), 0,
                    (struct sockaddr *)&recvadd, socklen) < 0){
                perror("Failed to reply to HEARTBEAT");
            }
        }
        std::cout << "Recvd: " << buff << std::endl;
        fflush(stdout);
    }
}

int UDPIntf::sender_monitor(uint32_t tid){
    std::cout << "SENDER MONITOR on thread: " << tid << std::endl;
    std::vector<struct client_record>::iterator it;
    uint32_t count = 0;
    std::unique_lock<std::mutex> m_lck(bcast_lck);
    m_lck.unlock();
    while(1){
        m_lck.lock();
        //std::cout << "elli idivi" << std::endl;
        while(bcast_signal != true){
            cv.wait(m_lck);
	//	sleep(1);
    	//	std::cout << "bcast_signal " << (bcast_signal ? "True" : "false") << std::endl;
        }
        std::cout << "elli banthu" << std::endl;
        // for Node in Nodes: spawn workers who can search subscription and
        // send UDP packets.
        count = 0;
        struct article msg = bcast_msg.front();
        bcast_msg.pop();
        for(it=_cli_obj->clientRecords.begin(); 
            it<_cli_obj->clientRecords.end(); ++it){
            std::cout << "DEBUG: Spawning thread for subs_check " 
                      << count<<  std::endl;
            thread_pool[count] = std::thread(&UDPIntf::send_or_die, this, 
                                             &it->article_info, 
                                             &it->node_info,
                                             msg, count);
            count++;  
        }
        for(int i=0; i<count; i++){
	        std::cout << "DEBUG: join thread for subs_check: "
                      << i << std::endl;
            thread_pool[i].join();
        }
        bcast_signal = false;
        m_lck.unlock();
    }
    
}

int UDPIntf::send_or_die(std::vector<struct article> *subscriptions, 
                    struct node *client,
                    struct article msg, uint32_t tid){
    std::cout << "Spawned worker: " << tid <<std::endl;
    if(_cli_obj->check_subscriber(subscriptions, msg) > 0){
        send_to_clients(client->ip, client->port,
                        strdup(_pl_obj->pack_to_string(&msg).c_str()));
        std::cout << "MSG sent to" << client->ip << std::endl;
    }
    
}

int UDPIntf::acquire_bcast_lock(){
    bcast_lck.lock();
    return 1;
}
int UDPIntf::acquire_bcast_unlock(){
    bcast_lck.unlock();
    return 1;
}


void UDPIntf::set_bcast_signal(bool val){
    bcast_signal = val;
}

void UDPIntf::notify(){
    std::cout << "notify kardvi " << std::endl;
    cv.notify_all();
    std::cout << "bcast_signal " 
              << (bcast_signal ? "True" : "false") << std::endl;
}

void UDPIntf::add_pub_msg(struct article *msg){
    bcast_msg.push(*msg);
}

int UDPIntf::get_port_number(){
    return portno;
}

uint16_t UDPIntf::get_registry_port(){
    return registry_port;
}
uint32_t UDPIntf::get_registry_ip(){
    return registry_ip;
}



int UDPIntf::deregister(){
    std::cout << "DeRegister RPCs to registry server";
    
    char *serverIp = CommonUtil::getMyIpAddress(); 
    int port  = get_port_number();
    int port_reg  = get_registry_port();

    std::string send = "";
    std::string prefix = "";
    
    prefix += std::string("Deregister") + ";" + std::string("RPC") + ";" + serverIp + ";" + std::to_string(port);
    send += prefix;
    
    send_to_clients(get_registry_ip(), htons(port_reg), strdup(send.c_str())); 
}
