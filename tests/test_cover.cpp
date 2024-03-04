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


 