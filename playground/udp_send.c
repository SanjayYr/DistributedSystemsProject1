/*
        demo-udp-03: udp-send: a simple udp client
    send udp messages
    sends a sequence of messages (the # of messages is defined in MSGS)
    The messages are sent to a port defined in SERVICE_PORT

        usage:  udp-send

        Paul Krzyzanowski
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVICE_PORT 5104
#define BUFLEN 1024
#define MSGS 5  /* number of messages to send */

int main(void)
{
    struct sockaddr_in myaddr, remaddr, dirt;
    int fd, i, slen=sizeof(remaddr);
    socklen_t addrlen = sizeof(remaddr);
    char *server = "128.101.35.147"; /* change this to use a different server */
    char buf[BUFLEN];

    /* create a socket */

    if ((fd=socket(AF_INET, SOCK_DGRAM, 0))==-1)
        printf("socket created\n");

    /* bind it to all local addresses and pick any port number */

    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  //  if (inet_aton("128.101.37.3", &myaddr.sin_addr)==0) {
    //    fprintf(stderr, "inet_aton() failed\n");
      //  exit(1);
    //}
    myaddr.sin_port = htons(INADDR_ANY);

    if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
        perror("bind failed");
        return 0;
    }       

    /* now define remaddr, the address to whom we want to send messages */
    /* For convenience, the host address is expressed as a numeric IP address */
    /* that we will convert to a binary format via inet_aton */

    memset((char *) &remaddr, 0, sizeof(remaddr));
    remaddr.sin_family = AF_INET;
    remaddr.sin_port = htons(SERVICE_PORT);
    if (inet_aton("128.101.37.9", &remaddr.sin_addr)==0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    /* now let's send the messages */

    for (i=0; i < 1; i++) {
        printf("Sending packet %d to %s port %d\n", i, server, SERVICE_PORT);
        sprintf(buf, "Register;RPC;128.101.37.3;1925;132;132");
        if (sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&remaddr, slen)==-1)
            perror("sendto");
        fflush(stdout);
//        int j;
  //      if((j = recvfrom(fd, buf, BUFLEN, 0, (struct sockaddr *)&dirt, &addrlen)) == -1){
    //       printf("Nothing received");
      //  }
        //printf("recvd: %s", buf);
    }
    

    for (i=0; i < 1; i++) {
        printf("Sending packet %d to %s port %d\n", i, server, SERVICE_PORT);
        sprintf(buf, "GetList;RPC;128.101.37.3;1925");
        if (sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&remaddr, slen)==-1)
            perror("sendto");
        fflush(stdout);
        i = recvfrom(fd, buf, BUFLEN, 0, (struct sockaddr *)&dirt, &addrlen);
        printf("recvd: %s\n", buf);
    }

  /*  while (1){
        printf("In while");
        i = recvfrom(fd, buf, BUFLEN, 0, (struct sockaddr *)&dirt, &addrlen);
        printf("recvd: %s\nsize: %d", buf, i);
        if (sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&remaddr, slen)==-1)
            perror("sendto");

   */ 
    close(fd);
    return 0;
}

