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
#include "proxy_ev_queues.h"
//hash helper:
#include "util/hash_helper.h"
//ktls helper:
#include "tls/ktls_wrapper.h"
//interface info:
#include "util/interface_info.h"
//conntrack listener:
#include "conntrack/conntrack_listener.h"

//tls helper:
#include "tls/tls_helper.h"
//iptables helper:



#include "conntrack/iptables_helper.h"


//SAMPLER,STRATEGY:
#include "sampler/sampler.h"


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

    //initialize the iptables helper

    //create libevent server and client
    //create the libevent server and client for the frontends and backends :


    //initialize the proactors/async event queues 

    happy_proxy::proactor_event_queues_manager manager;

    //set the tls globals:

    //conntrack listener: 
    //initialize the conntrack listener
    happy_proxy::conntrack::conntrack_listener listener;
    //add frontend policies, use acl to add policies :
    //add callback for the conntrack listener 

    //add backend callbacks



    //check certificate files on globals:
    auto dh_file =global.ssl_default_dh_param ;
    
    auto ecdh_curve = global.ssl_default_ecdh_curve;
 

    //test gtls wrapper first
    happy_proxy::tls_globals     gtls_globals;

    gtls_globals.dh_params_file_ = dh_file;
    gtls_globals.ecdh_curve_ = ecdh_curve;

    //set the tls globals:
    gtls_globals.ssl_tune_ssl_default_dh_params_ = true; 
    gtls_globals.ssl_default_bind_ciphers_ = "ECDHE-RSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384";
    gtls_globals.ssl_default_bind_options_ = {"no-sslv3", "no-tlsv10", "no-tlsv11"}; 
    gtls_globals.ssl_default_server_ciphers_ = "ECDHE-RSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384"; 
    gtls_globals.ssl_default_server_options_ = {"no-sslv3", "no-tlsv10", "no-tlsv11"}; 
    gtls_globals.ssl_default_client_ciphers_ = "ECDHE-RSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384";
    gtls_globals.ssl_default_client_options_ = {"no-sslv3", "no-tlsv10", "no-tlsv11"};
    gtls_globals.ssl_default_client_ctx_ = "client"; 
    //initialize the tls helper
    happy_proxy::tls_helper  helper(gtls_globals); 
    
 
    happy_proxy::tls::ktls_wrapper ktls;
    
    //initialize the ktls

    //initialize the ip address
    

    
    //check dh_file: 
    if(dh_file.empty())
    {
        std::cerr << "Error: missing dh file" << std::endl;
        return 1;
    }
    //check ecdh_curve:
    if(ecdh_curve.empty())
    {
        std::cerr << "Error: missing ecdh curve" << std::endl;
        return 1;
    }

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
        //print the front end data :
        std::cout << "Frontend Name: " << frontend.name << std::endl; 
        //print the front end data :
        std::cout << "=========================" << std::endl;
        for(auto& acl : frontend.acls)
        {
            std::cout << "ACL: " << acl << std::endl;
        }
        std::cout << "Bind: " << frontend.bind << std::endl;
        std::cout << "Mode: " << frontend.mode << std::endl;
        for(auto& option : frontend.options)
        {
            std::cout << "Option: " << option << std::endl;
        }
        std::cout << "=========================" << std::endl;
        
        happy_proxy::proxy_server* server = new happy_proxy::proxy_server(frontend, session_manager);
        servers.push_back(server);
    }
    for(auto& backend : proxy_config.get_backends())
    {
        //print the back end data :
        std::cout << "Backend Name: " << backend.name << std::endl;
        //print the back end data :
        std::cout << "=========================" << std::endl;
        std::cout << backend.mode << std::endl;

        for(auto& server : backend.servers)
        {
            std::cout << "Server: " << server << std::endl;
        }   

        happy_proxy::proxy_server* server = new happy_proxy::proxy_server(backend, session_manager);
        servers.push_back(server);
    }
    for(auto& server : servers)
    {
        server->start();
    }
    //init conntrack:
    //initialize the conntrack policies and queues for the frontends and backends 
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

    std::cout <<"adding iptables rules for the frontends and backends" << std::endl; 
    //use iptables helper to add rules 
    //add iptables rules for the frontends and backends 
    //iterate frontends:
 
    happy_proxy::iptables_helper iptables; 
    
     for(auto& frontend : proxy_config.get_frontends())
    { 
        
        auto acls = frontend.acls;
        for (auto& acl : acls)
        {
            std::cout << "ACL: " << acl << std::endl;
            //add iptables rule for the acl
            iptables.add_iptables_rule("filter", "INPUT", acl.c_str());
        }   
    }
    //add conntrack rules for the frontends and backends 


    //add ktls rules for the frontends and backends 

    //start the proxy clients 
    //add iptables rules for the frontends and backends 
 
    //add conntrack rules for the frontends and backends

    close (ktls_socket);
    return 0;

 
}//end of main

