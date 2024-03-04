#include <iostream>
#include <string>
#include <vector>

#include <map>
#include <set>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>
#include "proxy.h"

#include "proxy_util.h"

#include "proxy_session_manager.h"

//for AF_INET, AF_INET6, AF_UNSPEC
#include <sys/socket.h>
//for ipv4, ipv6
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>


namespace happy_proxy
{
    std::vector<int> proxy_client::get_downstream_ids()
    {
        return this->downstream_ids_;
    }
    std::vector<int> proxy_client::get_upstream_ids()
    {
        return this->upstream_ids_;
    }
   std::string proxy_client::get_proxy_server_protocol_type()
    {
        return this->proxy_server_protocol_type_;
    }
   std::string proxy_client::get_proxy_server_family()
    {
        return this->proxy_server_family_;
    }
  std::string  proxy_client::get_proxy_server_socket_type()
    {
        return this->proxy_server_socket_type_;
    }
   std::string proxy_client::get_proxy_server_protocol()
    {
        return this->proxy_server_protocol_;
    }
   std::string proxy_client::get_proxy_server_type()
    {
        return this->proxy_server_type_;
    }

   std::string proxy_client::get_proxy_server_port()
    {
      //  return std::stoi(this->proxy_server_port_);
        return this->proxy_server_port_;
    }
 std::string    proxy_client::get_proxy_server_password()
    {
        return this->proxy_server_password_;
    }
   std::string proxy_client::get_proxy_server_user_name()
    {
        return this->proxy_server_user_name_;
    }

    std::string proxy_client::get_proxy_server_host_name()
    {
        return this->proxy_server_host_name_;
    }
    std::string proxy_client::get_proxy_server_ip_address()
    {
        return this->proxy_server_ip_address_;
    }
    std::string proxy_client::get_password()
    {
        return this->password_;
    }
    std::string proxy_client::get_user_name()
    {
        return this->user_name_;
    }
    std::string  proxy_client::get_host_name()
    {
        return this->host_name_;
    }
   std::string  proxy_client::get_ip_address()
    {
        return this->ip_address_;
    }
    int proxy_client::get_port()
    {
        return this->port_;
    }
    int proxy_client::get_protocol_type()
    {
        return this->protocol_type_;
    }
    int proxy_client::get_socket_type()
    {
        return this->socket_type_;
    }
    int proxy_client::get_family_type()
    {
        return this->family_type_;
    }

    proxy_client::proxy_client()
    {
        this->family_type_ = AF_UNSPEC;
        this->socket_type_ = SOCK_STREAM;
        this->protocol_type_ = 0;
        this->port_ = 0;
        this->ip_address_ = "";
        this->host_name_ = "";
        this->user_name_ = "";
        this->password_ = "";
        this->proxy_server_ip_address_ = "";
        this->proxy_server_host_name_ = "";
        this->proxy_server_user_name_ = "";
        this->proxy_server_password_ = "";
        this->proxy_server_port_ = "";
        this->proxy_server_type_ = "";
        this->proxy_server_protocol_ = "";
        this->proxy_server_family_ = "";
        this->proxy_server_socket_type_ = "";
        this->proxy_server_protocol_type_ = "";
        this->running_ = false;
        this->log_file_ = "";
    }
    proxy_client::~proxy_client()
    {
        
    }  
}