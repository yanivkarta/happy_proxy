//implements happy_proxy:: server and client with libevent asynchronoous event handling 
//         

#ifndef HAPPY_PROXY_LIBEVENT_PROXY_IMPLEMENTATION_H_
#define HAPPY_PROXY_LIBEVENT_PROXY_IMPLEMENTATION_H_

#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/buffer_compat.h>
#include <event2/bufferevent_compat.h>
#include <event2/bufferevent_ssl.h>
#include <event2/bufferevent_struct.h>

//happy_proxy includes 
#include "../proxy.h"
#include "../proxy_config.h"
#include "../proxy_util.h"
#include "../proxy_session_manager.h"
#include "../proxy_server.h"
#include "../proxy_ev_queues.h"

//for tls ( gnutls ) :
#include <gnutls/gnutls.h>
#include <gnutls/x509.h>
#include <gnutls/crypto.h>
#include <gnutls/abstract.h>
#include <gnutls/ocsp.h>

//for our tls helper:
#include "../tls/tls_helper.h"


namespace happy_proxy
{
    class libevent_proxy_implementation : public proxy_server, public proxy_client
    {

        //use proactor event queues manager for libevent queues: 
        

    private:
        tls_helper tls_;
        struct event_base* base_;
        struct evconnlistener* listener_;
        
        happy_proxy::proxy_session_manager* session_manager_; 
        happy_proxy::proxy_config* config_;
        
        std::string log_file_;
        std::ofstream log_;
        std::mutex log_mutex_;
        std::mutex session_mutex_;

        //for libevent queues:
        happy_proxy::proactor_event_queues_manager* ev_queues_;
        std::mutex ev_queues_mutex_;
    public:
        //constructor:
        libevent_proxy_implementation(happy_proxy::proxy_config* config, const std::string& log_file)
            : config_(config), log_file_(log_file)
        {
            // Initialize libevent
            base_ = event_base_new();
            if (!base_) {
            throw std::runtime_error("Failed to initialize libevent");
            }

            // Create a listener for incoming connections
            struct sockaddr_in sin;
            memset(&sin, 0, sizeof(sin));
            sin.sin_family = AF_INET;
            sin.sin_addr.s_addr = htonl(INADDR_ANY);
            sin.sin_port = htons(config_->());

            listener_ = evconnlistener_new_bind(base_, acceptCallback, this,
            LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1,
            (struct sockaddr*)&sin, sizeof(sin));
            if (!listener_) {
            throw std::runtime_error("Failed to create listener");
            }

            // Initialize the session manager
            session_manager_ = new happy_proxy::proxy_session_manager();

            // Initialize the event queues manager
            ev_queues_ = new happy_proxy::proactor_event_queues_manager();

            // Open the log file
            log_.open(log_file_, std::ios::app);
            if (!log_.is_open()) {
            throw std::runtime_error("Failed to open log file");
            }
        }

    };
}





#endif 