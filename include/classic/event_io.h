#ifndef __HAPPY_PROXY_EVENT_IO_H_
#define __HAPPY_PROXY_EVENT_IO_H_

#include <string>
#include <vector>
#include <map>

#include <memory>
#include <mutex>
#include <thread>
#include <atomic>

#include <condition_variable>

#include <fstream>
#include <iostream>

#include <functional>
#include <iomanip>
#include <sstream>

#include <chrono>
#include <ctime>
#include <algorithm>

//libevent:
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event_struct.h>
#include <event2/event_compat.h>
#include <event2/event-config.h>
#include <event2/event_struct.h>
#include <event2/event_compat.h>


#include "../scather/scatter.h"
#include "../scather/gather.h"

//happy proxy interfaces and resources :
#include "proxy_util.h"
#include "proxy_session_manager.h"
#include "proxy_client.h"
#include "proxy_server.h"
#include "proxy_config.h"



namespace happy_proxy{

    class libevent_server : public proxy_server
    {
        //libevent server
        struct event_base* base;
        struct evconnlistener* listener;
        //for ipv4, ipv6:
        struct sockaddr_in sin;
        //libevent server thread
        std::thread server_thread;
        //libevent server mutex
        std::mutex server_mutex;
        //libevent server port
        int port;

        //ip families:
        int family;

        //libevent server id
        int64_t id;
        //libevent server session manager
        std::unique_ptr<proxy_session_manager> session_manager;
        //libevent server clients
        std::vector<const proxy_client*> clients;
        //libevent server config
        struct server_details config;
        char* buffer_;
        //libevent server buffer size
        size_t buffer_size_;
        //libevent server buffer mutex
        std::mutex buffer_mutex;
        //libevent server buffer condition
        std::condition_variable buffer_condition_;
        //libevent server buffer ready
        bool buffer_ready_;
        public:
        libevent_server();
        ~libevent_server();
        void start() override;
        void stop() override;
        


}





#endif // __HAPPY_PROXY_EVENT_IO_H_
