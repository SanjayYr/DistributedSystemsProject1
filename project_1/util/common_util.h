#ifndef __COMMON_UTIL__
#define __COMMON_UTIL__

#include <iostream>
#include <string>
#include <functional>
#include <memory.h>
#include "d_types.h"
#include <arpa/inet.h>
#include <cstdlib>
#include <cstdio>


class CommonUtil{

private:

public:
    // should call unpack string, not sure if we need this constructor as long
    // as we implement the next one.
    CommonUtil();
    // should parse the string and populate the s_msg struct;

    static char *getMyIpAddress(); 
    // destructor.
    ~CommonUtil();

};

#endif // __COMMON_UTIL__
