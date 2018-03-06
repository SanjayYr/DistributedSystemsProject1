#include "client_list.h"

ClientList::ClientList(){
}


//void ClientList::pp_nodes(uint32_t i){
//        std::cout << "NODE " << i<< " ip: "<<_ip_table.nodes[i].ip << std::endl
//                  << "port: " << _ip_table.nodes[i].port << std::endl
//                  << "status: "<<_ip_table.nodes[i].status << std::endl;
//
//}

int ClientList::add_client(uint32_t ip, uint16_t port){

    int size =(int) clientRecords.size();

    if(size>MAX_CLIENTS){
        return -1;
    }

    std::vector<struct client_record>::iterator it;
    for (it=clientRecords.begin(); it < clientRecords.end(); ++it){
        if(((struct client_record)*it).node_info.ip==ip &&
           ((struct client_record)*it).node_info.port==port){
            return 0;
        }
    }

    struct client_record clientRecord;
    clientRecord.node_info.ip = ip;
    clientRecord.node_info.port=port;
    clientRecords.push_back(clientRecord);

    return 0;
}

// Remove client
int ClientList::rem_client(uint32_t ip, uint16_t port){

    int size =(int) clientRecords.size();

    if(size==0){
        return -1;
    }

    std::vector<struct client_record>::iterator it;
    for (it=clientRecords.begin(); it < clientRecords.end(); ++it){
        if(((struct client_record)*it).node_info.ip==ip &&
           ((struct client_record)*it).node_info.port==port){
            it=clientRecords.erase(it);
            return 0;
        }
    }

    return -1;


}

int ClientList::add_subscription(uint32_t ip, uint16_t port, struct article *in_sub){
    std::vector<struct client_record>::iterator it;
    for (it=clientRecords.begin(); it<clientRecords.end(); ++it){
        if(((struct client_record)*it).node_info.ip==ip &&
           ((struct client_record)*it).node_info.port==port){
           it->article_info.push_back(*in_sub);
           std::cout << "Adding Subscription" << std::endl;
           //std::cout << it->article_info.back() << std::endl;
           
           std::cout << " cat:" << it->article_info.back().type 
                     << "\n orginator: " << it->article_info.back().originator
                     << "\n org: " << it->article_info.back().org
                     << "\n contents: " << it->article_info.back().contents<< std::endl;
                     
           return 1;
        }
    }
    return -1;
}

int ClientList::del_subscription(uint32_t ip, uint16_t port, struct article *in_sub){
    std::vector<struct client_record>::iterator it;
    std::vector<struct article>::iterator artit;
    for (it=clientRecords.begin(); it<clientRecords.end(); ++it){
        if(((struct client_record)*it).node_info.ip==ip &&
           ((struct client_record)*it).node_info.port==port){
            
            for(artit=it->article_info.begin(); 
                    artit<it->article_info.end(); ++artit){
                if (in_sub->hash == artit->hash){

                    it->article_info.erase(artit);
                    std::cout << "Erased article subscription" << std::endl;
                
                }
                
                /*    
                if(artit->type == in_sub->type && 
                    (strcmp(artit->originator, in_sub->originator) == 0) &&
                    (strcmp(artit->org, in_sub->org)) &&
                    (strcmp(artit->contents, in_sub->contents) == 0)){
                    
                    it->article_info.erase(artit);
                    std::cout << "Erased article subscription" << std::endl;
                }*/
            }
        }
    }

}

int ClientList::check_subscriber(std::vector<struct article> *subscriptions, 
                                    struct article msg ){
    //return 1;
    // std::cout << "DEBUG: Entered check subscriber" << std::endl;
//return 1;
    if(subscriptions!=NULL && !subscriptions->empty() && 
            msg.type == ALL && strlen(msg.org)==0 && strlen(msg.originator)==0){
        std::cout << "DEBUG: Check passed wildcard case" << std::endl;
        return 1;
    }

    if(subscriptions==NULL){
        std::cout << "DEBUG: Subscriptions list is NULL" << std::endl;
        return -1;
    }

    if(subscriptions->empty()){
        std::cout << "DEBUG: Subscription list is empty" << std::endl;
        return -1;
    }

    std::vector<struct article>::iterator it;
    //TODO - Check again
    for(it= subscriptions->begin();it<subscriptions->end();++it) {
        //struct article check_article = *it;
        
        if (it->type == msg.type || (msg.type == ALL) ||
            (it->type == ALL)) {
                    std::cout << "Passed MSG TYPE" << std::endl;
            if (strcmp(it->originator, msg.originator)==0 || 
                (strlen(msg.originator) == 0) ||
                (strlen(it->originator) == 0)) {
                    std::cout << "Passed ORIGINATOR" << std::endl;
                if (strcmp(it->org, msg.org) == 0 || 
                    (strlen(msg.org) == 0) || (strlen(it->org) == 0)) {
                    std::cout << "Passed ORG" << std::endl;
                    std::cout << " type: " << it->type << std::endl
                              << " orginator: " << it->originator << std::endl
                              << " org: " << it->org << std::endl
                              << " COMPARE orgs: "<< strcmp(it->originator, 
                                                    msg.originator)<<std::endl
                              << " strlen originator: "
                              << strlen(msg.originator) << std::endl;
                    return 1;
                }
            }
        }
    }

    return -1;
  }

// TODO(Harshith): This method should call your _lookup() and not append client
// if it already exists.
//int ClientList::add_client(uint32_t ip, uint16_t port){
//    int slot = _get_nextslot();
//    if (slot != -1){
//        _ip_table.nodes[slot].ip = ip;
//        _ip_table.nodes[slot].port = port;
//        _ip_table.live_status[slot/32] |= (1 << slot % 32);
//
//    }
//}

//int ClientList::rem_client(uint32_t ip, uint16_t port){
//    struct node del_node;
//    int index;
//    del_node.ip = ip;
//    del_node.port = port;
//
//    index = _lookup_node(del_node);
//    if (index != -1){
//        _ip_table.nodes[index].ip = INVALID_IP;
//        _ip_table.nodes[index].port = INVALID_PORT;
//        _ip_table.nodes[index].status = false;
//        return 0;
//    }
//    _ip_table.live_status[index] &= (~(1<<(index%32)));
//    std::cout << "Remove Client Failed at ClientList" << std::endl;
//    return -1;
//}

//int ClientList::add_subscription(struct s_msg *in_sub){
//    subs_table.push_back(*in_sub);
//    return 0;
//}
//
//int ClientList::_lookup_node(struct node){
//    return 0;
//}
