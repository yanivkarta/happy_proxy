#include <cstdio>
#include <string>
//for pid_t
extern "C"{
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
}
//for event loop
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


//for proxy
#include "proxy.h"
#include "proxy_config.h"
#include "proxy_util.h"
#include "proxy_server.h"
#include "proxy_client.h"
#include "proxy_session.h"
#include "proxy_session_manager.h"
//hash helper:
#include "util/hash_helper.h"
//ktls helper:
#include "tls/ktls_wrapper.h"
//interface info:
#include "util/interface_info.h"


//iptables helper:

#include "conntrack/iptables_helper.h"


// HAPPY Proxy initialization. 
//Loads the configurations from local or service/daemon and initializes the proxy. 
//This is a blocking call and will not return until the proxy is initialized. 

//Architecture of the proxy,with asynchronous event queues, is based on the event loop pattern. 
//The proxy is composed of a server, client, and session manager. 
//The server listens for incoming connections and forwards them to the session manager. 
//The client connects to the session manager and forwards the data to the server. 
//The session manager manages the sessions and forwards the data between the server and client. 
//The proxy is initialized by loading the configurations from local or service/daemon. 
//The configurations are loaded with HAPROXY configuration files. 
//The proxy is initialized with the server, client, and session manager. 
 
int main(int argc, char** argv)
{
    
    //parse the command line arguments 
    //print usage if the arguments are not valid 
    bool is_daemon = false;   
    happy_proxy::interface_info interfaces;


    std::string config_file = "happy_proxy.cfg";
    for(int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "-d") == 0)
        {
            is_daemon = true;
        }
        else if(strcmp(argv[i], "-c") == 0)
        {
            if(i + 1 < argc)
            {
                config_file = argv[i + 1];
                i++;
            }
            else
            {
                std::cerr << "Error: missing config file" << std::endl;
                return 1;
            }
        }
        else
        {
            std::cerr << "Error: invalid argument" << std::endl;
            return 1;
        }
    }   
    if ( is_daemon)
    {
        //set default config path to /etc/happy_proxy.cfg  
       
        if(config_file.empty())
        {
            config_file = "/etc/happy_proxy.cfg";
        }
        //check if the config file exists
        std::ifstream file(config_file);
        if(!file.good())
        {
            std::cerr << "Error: config file does not exist" << std::endl;
            return 1;
        }
        file.close(); 
        //daemonize the process 
        pid_t pid = fork();
        if(pid < 0)
        {
            std::cerr << "Error: fork failed" << std::endl;
            return 1;
        }
        if(pid > 0)
        {
            return 0;
        }
        umask(0);
        pid_t sid = setsid();
        if(sid < 0)
        {
            std::cerr << "Error: setsid failed" << std::endl;
            return 1;
        }
        if(chdir("/") < 0)
        {
            std::cerr << "Error: chdir failed" << std::endl;
            return 1;
        }
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);   
    }
    
    //initialize the proxy
    happy_proxy::proxy_config proxy_config(config_file);
    
    proxy_config.initialize();
    //get the backend/frontend servers from the proxy config :
   
    for (auto& frontend : proxy_config.get_frontends())
    {
        std::cout << "Frontend Name: " << frontend.name << std::endl;
        //print the front end data : 
        std::cout << "=========================" << std::endl;
        for (auto& acl : frontend.acls)
        {
            std::cout << "ACL: " << acl << std::endl;
        }
        std::cout<<"Bind:"<<frontend.bind << std::endl;
        std::cout<<"Mode:"<<frontend.mode << std::endl;
        for ( auto& option : frontend.options)
        {
            std::cout<<"Option: "<<option << std::endl;
        }
        std::cout << "=========================" << std::endl;

    }
    for (auto& backend : proxy_config.get_backends())
    {
        std::cout << "Backend Name: " << backend.name << std::endl;
        //print the back end data : 
        std::cout << "=========================" << std::endl;
        std::cout<<backend.mode << std::endl;
        for (auto& server : backend.servers)
        {
            std::cout << "Server: " << server << std::endl;
        }
    }   

    //print the globals: 
    std::cout << "Globals" << std::endl;
    std::cout << "=========================" << std::endl;
    const auto& global = proxy_config.get_globals();
    global.print();
    std::cout << "=========================" << std::endl;

    auto euid = geteuid();
    if(euid==0)
    {
        std::cout << "Running as root" << std::endl;
    }
    else
    {
        std::cerr << "Running as non-root" << std::endl; 
        
    }
    
    try{
    happy_proxy::iptables_helper iptables; 
    }
    catch(happy_proxy::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    catch(std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Error: unknown exception" << std::endl;
    } 

    happy_proxy::tls::ktls_wrapper ktls;
    
    //initialize the ktls

    //initialize the ip address
    

    
    
    //register the iptables rules:
    //register the iptables rules for the frontends and backends
    //register the conntrack rules for the frontends and backends
    //register the ktls rules for the frontends and backends

    //initialize the proxy
    std::vector<happy_proxy::proxy_server*> servers;
    std::vector<happy_proxy::proxy_client*> clients;
    
    happy_proxy::proxy_session_manager session_manager;
    std::vector<happy_proxy::tls::ktls_session*> ktls_sessions; 

    //setup IPTABLES policies and queues 
    //setup the iptables policies and queues for the frontends and backends
    for(auto& frontend : proxy_config.get_frontends())
    {
        happy_proxy::proxy_server* server = new happy_proxy::proxy_server(frontend, session_manager);
        servers.push_back(server);
    }
    for(auto& backend : proxy_config.get_backends())
    {
        happy_proxy::proxy_server* server = new happy_proxy::proxy_server(backend, session_manager);
        servers.push_back(server);
    }
    for(auto& server : servers)
    {
        server->start();
    }
    //init conntrack:
    //initialize the conntrack policies and queues for the frontends and backends 

 
    //start the proxy servers
    //add iptables rules for the frontends and backends 
    
    //test ktls socket
    int ktls_socket = ktls.ktls_socket( AF_INET, SOCK_STREAM, 0); 

    if(ktls_socket < 0)
    {
        std::cerr << "Error: failed to create ktls socket" << std::endl;
        return 1;
    }
    else
    {
        std::cout << "KTLS socket created" << std::endl;
   
    }

    close (ktls_socket);
    return 0;
}
