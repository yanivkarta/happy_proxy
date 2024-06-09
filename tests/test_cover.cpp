#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>
#include <fstream>

#include "proxy.h"
#include "proxy_config.h"
#include "proxy_session_manager.h"
#include "proxy_util.h"
#include "proxy_server.h"
 
#include "proxy_ev_queues.h"
#include "aio/aio_control.h"

//test configuration:

//test async event queues manager

void test_async_event_queues_manager()
{
    //parameters
    happy_proxy::async_event_param epoll_param;
    happy_proxy::async_event_param select_param;
    happy_proxy::async_event_param poll_param;
    happy_proxy::async_event_param event_param;
    happy_proxy::async_event_param event_base_param;
    
    //test epoll
    happy_proxy::event_queues_manager manager;
    manager.remove_epoll(&epoll_param);
    manager.wait_epoll(&epoll_param);
    manager.get_epoll_fd();
    return ;
}//end of test_async_event_queues_manager
void test_acls()
{
    //test acls
    //parse acl from line like : acl forbidden_dst url_ip
    //in the context of config file ( frontend, backend, etc)
    std::string line = "acl forbidden_dst url_ip";
    std::map<std::string,std::string> acl = happy_proxy::parse_acl(line); 
    return ;
}//end of test_acls


 void test_unsupervised_classification()
 {

    //TBD : 
    //test unsupervised classification
    //create a matrix to encapsulate stream data features 
    //create a model to classify the data 
    //test the model
    //test the model with a new data
    
     return ;
 }//end of test_unsupervised_classification 