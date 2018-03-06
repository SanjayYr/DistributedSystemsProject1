#include <iostream>
#include "d_types.h"
#include "client_list.h"

#include <arpa/inet.h>

int main (){
    ClientList *obj = new ClientList();
    
    struct node x;
    x.ip = inet_addr("192.168.1.1");
    x.port = htons(9211);

    obj->add_client(x.ip, x.port);
    obj->add_client(x.ip, x.port);
    obj->add_client(x.ip, x.port);
    obj->pp_nodes(0);
    obj->pp_nodes(1);
    obj->pp_nodes(2);
    obj->pp_nodes(3);
    obj->rem_client(x.ip, x.port);
    obj->rem_client(x.ip, x.port);
    obj->pp_nodes(0);
    obj->pp_nodes(1);
    obj->pp_nodes(2);
    obj->pp_nodes(4);


}
