#include "payload_util.h"

// TODO: Implement everything here

PayLoad::PayLoad(){
}

PayLoad::PayLoad(char *ip, char *port, char* raw_input){
    node_info.ip=inet_addr(ip);
    node_info.port=htons((uint16_t)atoi(port));
    memcpy(&payload_raw,raw_input,120);

    char* read_type;
    if(raw_input!=NULL){
        sscanf(raw_input,"%s;%s;%s;%s",read_type,article_info.originator,article_info.org,article_info.contents);
        int i;
        for(i=0;i<8;i++){
            if(msg_category[i].compare(read_type)==0)
                break;
        }
        article_info.type = static_cast<emsg_category>(i);
    }

}


PayLoad::PayLoad(char* raw_input){
    char *ip,*port,*read_type;

    if(raw_input!=NULL)
        sscanf(raw_input,"%s;%s;%s;%s;%s;%s",ip,port,read_type,article_info.originator,article_info.org,article_info.contents);
        node_info.ip = inet_addr(ip);
        node_info.port = htons((uint16_t)atoi(port));

	  int i;
      for(i=0;i<8;i++){
          if(msg_category[i].compare(read_type)==0)
              break;
      }
      article_info.type = static_cast<emsg_category>(i);
}

struct article PayLoad::parse_article(char *raw_input){
    struct article ret_article;
    memcpy(&payload_raw, raw_input, strlen(raw_input));
    char *read_type;
    char *ip = (char *)malloc(sizeof(char)*20);
    char *port = (char *)malloc(sizeof(char)*5);
    std::string raw_str;
    if(raw_input !=  NULL){

        raw_str.assign(raw_input, strlen(raw_input));
        
        std::string::size_type i = 0;
        std::string::size_type j = raw_str.find(';');

        read_type = strdup(raw_str.substr(i, j-i).c_str());
        i = ++j;
        j = raw_str.find(';', j);

        ret_article.originator = strdup(raw_str.substr(i, j-i).c_str());
        i = ++j;
        j = raw_str.find(';', j);
        
        ret_article.org = strdup(raw_str.substr(i, j-i).c_str());
        i = ++j;
        j = raw_str.find(';', j);
        
        ret_article.contents = strdup(raw_str.substr(i, raw_str.length()).c_str());

/*
        sscanf(raw_input, "%s;%s;%s;%s",read_type,
                                        ret_article.originator,
                                        ret_article.org, 
                                        ret_article.contents);
  */
        //ret_article.ip = inet_addr(ip);
        //ret_article.port = htons(atoi(port));
        /*
        std::cout << "type: " << read_type
                  << "\noriginator: " << ret_article.originator
                  << "\norg: " << ret_article.org
                  << "\ncontent: " << ret_article.contents << std::endl;
        */
        for (int i=0; i < 9; i++){
           if(strlen(read_type) < 1){
            ret_article.type=static_cast<emsg_category>(ALL);
            break;
           }
           if(msg_category[i].compare(read_type)==0){
            ret_article.type=static_cast<emsg_category>(i);
            break;
           } 
        }
    }
    ret_article.hash = hash_fn(raw_input);
    return ret_article;
}



string PayLoad:: get_type(){
    return msg_category[article_info.type];
}

string PayLoad::get_org(){
    return article_info.org;
}

string PayLoad::get_originator(){
    return article_info.originator;
}

string PayLoad::get_contents(){
    return article_info.contents;
}

string PayLoad::pack_to_string(){
    string temp ="";
    temp+= get_type()+";"+get_originator()+";"+get_org()+";"+get_contents();
    return temp;
}

string PayLoad::pack_to_string(struct article *msg){
    string temp ="";
    if(msg != NULL){
       
       temp+= msg_category[msg->type] +";"+ msg->originator + ";"+ msg->org+";"+msg->contents;
    }
    return temp;
}
