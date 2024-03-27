#ifndef HAPPY_PROXY_SERVER_H
#define HAPPY_PROXY_SERVER_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>

#include "proxy_util.h"
#include "proxy_session.h"

#include "proxy_client.h"
#include "proxy_config.h"

namespace happy_proxy
{
    // base class for proxy server
    // this class is responsible for starting and stopping each proxy subserver
    class proxy_session_manager;
    
    class proxy_server
    {
    protected:
        proxy_session_manager* session_manager_;
        std::vector<const proxy_client*> clients_;
        int port_;
        //settings : 
        struct server_details config_;
        std::mutex server_mutex_;
        
        std::atomic<bool> running_;
        std::vector<std::thread> threads_;

    public:
        proxy_server(server_details &config);
        proxy_server(server_details &config, proxy_session_manager* session_manager);
        //with frontend 
        proxy_server(const front_end& frontend,const proxy_session_manager& session_manager); 
        proxy_server(const back_end& backend,const proxy_session_manager& session_manager); 
        ~proxy_server();
        // Starts the proxy server.
        void start();
        // Stops the proxy server.
        void stop();
        // Returns the port number of the proxy server.
        int get_port()const;
        //
        void set_port(int port);
         // Returns the proxy client.
        std::vector<const happy_proxy::ProxyClient *> GetClients() const;
        // Returns the proxy session manager.
        const happy_proxy::proxy_session_manager *GetSessionManager() const;
        //initialize the proxy server when ready to start
        virtual void initialize() ;

        void run();
    };
}// namespace happy_proxy

#endif