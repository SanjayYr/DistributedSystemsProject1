#ifndef __CLIENT_LIST__
#define __CLIENT_LIST__

#include <iostream>
#include <vector>
#include <math.h>
#include "../util/d_types.h"

const int MAX_SUBSCRIPTION =            20;
const int MAX_CLIENTS =          MAX_NODES;

const uint32_t INVALID_IP =     0xffffffff;
const uint32_t INVALID_PORT =       0xffff;

class ClientList{

private:
    // set ip->0xffffffff, port->0xffff if invalid or when leave() method is
    // called.
    //int _get_nextslot();
    int _lookup_node(struct node);

public:
    std::vector<struct client_record> clientRecords;
    // as and when subscription request come, add an entry to subs_table.
    // initialize subs_table and _ip_table
    ClientList();

    // add client. Join rpc method should call this function and add entry to
    // _ip_table.
    int add_client(uint32_t ip, uint16_t port);

    // Remove client
    int rem_client(uint32_t ip, uint16_t port);


    // add subscription, your subscribe rpc should invoke this method somehow.
    // todo this you need to simply pass the object as a void* and cast as
    // ClientList there, should be very easy.
    int add_subscription(uint32_t ip, uint16_t port, struct article *in_sub);

    // delete subscription
    int del_subscription(uint32_t ip, uint16_t port, struct article *in_sub);
    
    // Checks for a publish rule to match input, subscribers list,
    // returns 1, if exists else returns false.
    int check_subscriber(std::vector<struct article> *subscriptions, 
                         struct article msg );
    
    // Prints nodes
    void pp_nodes(uint32_t i);


};

#endif//__CLIENT_LIST__
