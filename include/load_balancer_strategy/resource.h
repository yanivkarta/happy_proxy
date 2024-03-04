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

//resource client:

//resource session:
//resource route:
//resource auth:
//resource config:
//resource util:


}//namespace happy_proxy
#endif // RESOURCE_H_