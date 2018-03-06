#include "udp_handler.h"

UDPReceiver::UDPReceiver(uint16_t port){
    std::cout << "UDP listner on Port: " << port << std::endl;
    fd_rx = socket(AF_INET, SOCK_DGRAM, 0);

    if(fd_rx < 0){
        perror("OS did not give an fd for UDP rx socket");
    }
    optval = 1;
    setsockopt(fd_rx, SOL_SOCKET, SO_REUSEADDR, 
               (const void *)&optval , sizeof(int));

    clientadd.sin_family = AF_INET;
    clientadd.sin_addr.s_addr =  htonl(INADDR_ANY);
    clientadd.sin_port = htons(port);

    if(bind(fd_rx, (struct sockaddr *)&clientadd,
        sizeof(clientadd)) < 0){
        perror("ERROR on binding");
    }

    this->recv_thread = std::thread(&UDPReceiver::receive_monitor, this, 99);

}

int UDPReceiver::receive_monitor(uint32_t tid){
    std::cout << "receiver thread created TID: " << tid << std::endl;
    int len;
    socklen_t socklen = sizeof(clientadd);
    struct sockaddr_in recvadd;
    memset((char *)&recvadd, 0, sizeof(recvadd));
    while(1){
        len = recvfrom(fd_rx, buf, kBUFF_SIZE, 0, 
                        (struct sockaddr *)&recvadd, &socklen);
        if(len > 0){
            std::cout << "Received: " << buf << "\n--" 
                       << "From:" << inet_ntoa(recvadd.sin_addr) << std::endl;
        }
    }
}

int UDPReceiver::join_all(){
    this->recv_thread.join();
}
