#ifndef __PAYLOAD_UTIL__
#define __PAYLOAD_UTIL__

#include <iostream>
#include <string>
#include <functional>
#include <memory.h>
#include "d_types.h"
#include <arpa/inet.h>
#include <cstdlib>
#include <cstdio>


using namespace std;

// I don't think we need to implement all the methods here.
// Server to should be able to get_clients_by_[category|source|org].

class PayLoad{

private:
    struct node node_info;
    struct article article_info;
    char payload_raw[120];
    std::hash<std::string> hash_fn;
   //string _pack_string();

public:
    // should call unpack string, not sure if we need this constructor as long
    // as we implement the next one.
    PayLoad();
    // should parse the string and populate the s_msg struct;
    PayLoad(char *ip, char *port, char* raw_input);

    PayLoad(char* raw_input);

    struct article parse_article(char *raw_input);

    string get_type();
    string get_org();
    string get_originator();
    string get_contents();

    // Updates the object with new string, not sure if we need to implement
    // this.
    // serializes to string beautifully. UDP_send method call this before 
    // deciding to send it to the recepients.
    string pack_to_string();
    string pack_to_string(struct article *msg);
    
    // destructor.
    ~PayLoad();

};

#endif // __PAYLOAD_UTIL__
