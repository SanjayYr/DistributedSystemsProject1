/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */
#include <iostream>
#include <arpa/inet.h>

#include "rpc_gen.h"
#include "group_server/obj_ptr.h"

int *
joinserver_1_svc(char *IP, int ProgID, int ProgVers,  struct svc_req *rqstp)
{
	static int  result;

	/*
	 * insert server code here
	 */
//     udp_obj->register_server();
     fflush(stdout);
     std::cout << "Join Server was Called" << std::endl;

	return &result;
}

int *
leaveserver_1_svc(char *IP, int ProgID, int ProgVers,  struct svc_req *rqstp)
{
	static int  result;

	/*
	 * insert server code here
	 */

	return &result;
}

int *
join_1_svc(char *IP, int Port,  struct svc_req *rqstp)
{
	static int  result;

    if (IP != NULL && Port > 0){
        if(clients_obj->add_client(inet_addr(IP), htons(Port)) != -1){
            result = 1;
            std::cout << "Clients are added: " << IP << std::endl;
        } else {
            result = 0;
            std::cout << "Error adding client @ join" << std::endl;
        }
    }
    fflush(stdout);

	/*
	 * insert server code here
	 */

	return &result;
}

int *
leave_1_svc(char *IP, int Port,  struct svc_req *rqstp)
{
	static int  result;
    if (IP && Port > 0){
        if(clients_obj->rem_client(inet_addr(IP), htons(Port)) != -1){
            result = 1;
            std::cout << "Client deleted: " << IP << std::endl;
        }
    }

	/*
	 * insert server code here
	 */

	return &result;
}

int *
subscribe_1_svc(char *IP, int Port, char *Article,  struct svc_req *rqstp)
{
	static int  result;
    struct article msg;
    if(IP && Port > 0 && Article){
        msg = pay_load_obj->parse_article(Article);
        if(msg.type == ALL && msg.originator == NULL && msg.org == NULL)
        {
           result = -1;
           return &result;
        }
        clients_obj->add_subscription(inet_addr(IP), htons(Port), &msg);
        std::cout << "Subscribe was called by: " << IP << std::endl;
        std::cout << "Article: " << Article <<std::endl;
        result = 1;
        fflush(stdout);
        return &result;
    }
    fflush(stdout);
    result = 0;
	/*
	 * insert server code here
	 */

	return &result;
}

int *
unsubscribe_1_svc(char *IP, int Port, char *Article,  struct svc_req *rqstp)
{
	static int  result;
    struct article msg;
    if(IP && Port > 0 && Article){
        msg = pay_load_obj->parse_article(Article);
        clients_obj->del_subscription(inet_addr(IP), htons(Port), &msg);
        std::cout << "UnSubscribe called for: " << Article << std::endl;
        result = 1;
        fflush(stdout);
        return &result;
    }
    
	/*
	 * insert server code here
	 */

	return &result;
}

int *
publish_1_svc(char *Article, char *IP, int Port,  struct svc_req *rqstp)
{
	static int  result;
    struct article msg;
        std::cout << "Publish called for: " << Article << std::endl;
    if(IP && Port > 0 && Article){
        msg = pay_load_obj->parse_article(Article);
        udp_obj->acquire_bcast_lock();     
        //std::unique_lock<std::mutex> m_lck(udp_obj->bcast_lck);
	    //(udp_obj->bcast_msg).push(msg);
	    udp_obj->add_pub_msg(&msg);
        udp_obj->set_bcast_signal(true);
        udp_obj->acquire_bcast_unlock();
        //m_lck.unlock();
	    udp_obj->notify(); 
        
        std::cout << "Publish called for: " << Article << std::endl;
        result = 1;
        fflush(stdout);
        return &result;
    }
    
	/*
	 * insert server code here
	 */

	return &result;
}

int *
publishserver_1_svc(char *Article, char *IP, int Port,  struct svc_req *rqstp)
{
	static int  result;

	/*
	 * insert server code here
	 */

	return &result;
}

int *
ping_1_svc(struct svc_req *rqstp)
{
	static int  result;
    result = 1;

	/*
	 * insert server code here
	 */

	return &result;
}