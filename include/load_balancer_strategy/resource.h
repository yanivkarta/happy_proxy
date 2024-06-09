#ifndef RESOURCE_H_ 
#define RESOURCE_H_

#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <mutex>

#include <thread>
#include <atomic>
#include <fstream>
#include <iostream>

#include <functional>
//for formatting
#include <iomanip>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>

#include <syslog.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "proxy_util.h"
#include "proxy_session_manager.h"
#include "proxy_client.h"
#include "proxy_server.h"
#include "proxy_config.h"


namespace happy_proxy{

//define proxy resources and context. 
//resource category: server,backend,client,session,route,auth,config,util 
//resource type: server,backend,client,session,route,auth,config,util 

//resource server:
struct server_resource{
    //server details
    struct server_details config;
    //server session manager 
    std::unique_ptr<proxy_session_manager> session_manager; 
    //server clients
    std::vector<const proxy_client*> clients;
    //server mutex
    std::mutex server_mutex;
    //server port
    int port;

    int64_t id;
    
    };

//resource backend:
struct backend_resource{
    //backend details
    struct backend_details config;
    //backend session manager 
    std::unique_ptr<proxy_session_manager> session_manager; 
    //backend clients
    std::vector<const proxy_client*> clients;
    //backend mutex
    std::mutex backend_mutex;
    //backend port
    int port;
    int64_t id;
    };

struct client_resource{
    //client details
    struct client_details config;
    //client session manager 
    std::unique_ptr<proxy_session_manager> session_manager; 
    //client clients
    std::vector<const proxy_client*> clients;
    //client mutex
    std::mutex client_mutex;
    //client port
    int port;
    int64_t id;
    };  
    struct static_route_details{
    //route details
    std::string route_name;
    std::string route_type;
    std::string route_protocol;
    std::string route_family;
    std::string route_socket_type;
    std::string route_protocol_type;
    std::string route_host_name;
    std::string route_ip_address;
    std::string route_port;
    std::string route_user_name;
    std::string route_password;
    std::string route_auth_type;
    std::string route_auth_name;
    std::string route_auth_password;
    std::string route_auth_protocol;
    std::string route_auth_family;
    std::string route_auth_socket_type;
    std::string route_auth_protocol_type;
    std::string route_auth_host_name;
    std::string route_auth_ip_address;
    std::string route_auth_port;
    std::string route_auth_user_name;
    std::string route_auth_password;
    //time to live
    int ttl;
    int64_t id;

    };
    struct static_route_resource{
    //route details
    struct static_route_details config; 
    //route session manager
    std::unique_ptr<proxy_session_manager> session_manager; 
    //route clients
    std::vector<const proxy_client*> clients;
    //route mutex
    std::mutex route_mutex;
    //route port
    int port;
    int64_t id;
    };

    struct auth_details{
    //auth details
    std::string auth_name;
    std::string auth_type;
    std::string auth_protocol;
    std::string auth_family;
    std::string auth_socket_type;
    std::string auth_protocol_type;
    std::string auth_host_name;
    std::string auth_ip_address;
    std::string auth_port;

    std::string auth_user_name;
    std::string auth_password;
    int64_t id;
    };

    struct auth_resource{
    //auth details
    struct auth_details config;
    //auth session manager
    std::unique_ptr<proxy_session_manager> session_manager;
    //auth clients
    std::vector<const proxy_client*> clients;
    //auth mutex
    std::mutex auth_mutex;
    //auth port
    int port;
    int64_t id;
    };

    
}//namespace happy_proxy
#endif // RESOURCE_H_