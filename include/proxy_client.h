#ifndef HAPPY_PROXY_CLIENT_H
#define HAPPY_PROXY_CLIENT_H

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
#include "proxy_server.h"
#include "proxy_session_manager.h"


namespace happy_proxy
{
    class proxy_client
    {
    private:
        //family type:
        //AF_INET: IPv4
        //AF_INET6: IPv6
        //AF_UNSPEC: IPv4 or IPv6
        int family_type_; 
        int socket_type_;
        int protocol_type_;
        int port_;
        std::string ip_address_;
        std::string host_name_;
        std::string user_name_;
        std::string password_;
        std::string proxy_server_ip_address_; 
        std::string proxy_server_host_name_;

        std::string proxy_server_user_name_;
        std::string proxy_server_password_;
        std::string proxy_server_port_;
        std::string proxy_server_type_;
        std::string proxy_server_protocol_;

        std::string proxy_server_family_;
        std::string proxy_server_socket_type_;
        std::string proxy_server_protocol_type_;
        std::vector<int> upstream_ids_;
        std::vector<int> downstream_ids_;
        std::mutex client_mutex_;
        std::atomic<bool> running_;
        std::string log_file_;
        /* data */
    public:
        //get family type: 
        //AF_INET: IPv4
        //AF_INET6: IPv6 
        //AF_UNSPEC: IPv4 or IPv6
        int get_family_type(); 
        int get_socket_type(); 
        int get_protocol_type();
        int get_port();
        std::string get_ip_address();
        std::string get_host_name();
        std::string get_user_name();
        std::string get_password();
        std::string get_proxy_server_ip_address();
        std::string get_proxy_server_host_name();
        std::string get_proxy_server_user_name();
        std::string get_proxy_server_password();
        std::string get_proxy_server_port();
        std::string get_proxy_server_type();
        std::string get_proxy_server_protocol();
        std::string get_proxy_server_family();
        std::string get_proxy_server_socket_type();
        std::string get_proxy_server_protocol_type();
        //get upstream ids 
        std::vector<int> get_upstream_ids(); 
        //get downstream ids
        std::vector<int> get_downstream_ids();
        //get client mutex
        proxy_client(/* args */);
        ~proxy_client();
    };
    


    class ProxyClient : public proxy_client 
    {
        public:
        ProxyClient();
        ~ProxyClient();

    };
    
}   // namespace happy_proxy

#endif
