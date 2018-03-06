#ifndef __PROJECT_DTYPES__
#define __PROJECT_DTYPES__

#include <cstdint>
#include <string.h>
#include <stdint.h>
#include <string>
#include <vector>

#define MAX_ARTICLE_SIZE 120
#define MAX_NODES 100

using namespace std;

struct node{
    uint32_t ip;
    uint16_t port;
    // used to maintain status article delivery to subscribers(publish method)
    // this is needed only to ensure we don't send a given article more than
    // once to the subscriber as multiple subscription rule(s) may overlap.
    // should reset on completion of publish method
    bool status;
};

//Deprecate this
//struct ip_table{
//    struct node nodes[MAX_NODES];
//    // bit vector that is intended to maintain the status of ip entries in the
//    // ip table.
//    uint32_t live_status[MAX_NODES/32 +1];
//};

// payload fields are
// TYPE - str
// ORIGNATOR - str
// ORG - str
// CONTENTS - str

// type
enum emsg_category {
    SPORTS = 0,
    LIFESTYLE,
    ENTERTAINMENT,
    BUSINESS,
    TECHNOLOGY,
    SCIENCE,
    POLITICS,
    HEALTH,
    ALL
};

const string msg_category[9] = {"Sports",
                                "Lifestyle",
                                "Entertainment",
                                "Business",
                                "Technology",
                                "Science",
                                "Politics",
                                "Health",
                                "ALL"};

struct article{
    enum emsg_category type;
    char *originator;
    char *org;
    char *contents;
    long long hash;
};

struct client_record{
    struct node node_info;
    std::vector<struct article> article_info;
};

// TODO:Write method that parses the payload string and populates these fields.
//Deprecated
//struct s_msg{
//    int id;
//    struct node node_info;
//    enum emsg_category type;
//    char *originator;
//    char *org;
//    char *contents;
//};


#endif // __PROJECT_DTYPES__
