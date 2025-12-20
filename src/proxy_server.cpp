

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
#include "proxy_config.h"
#include "proxy_session_manager.h"
#include "proxy_util.h"
#include "proxy_server.h"

namespace happy_proxy
{
    //proxyserver implementation

    proxy_server::proxy_server(  happy_proxy::server_details& config, happy_proxy::proxy_session_manager* session_manager)
    {
        this->config_ = config;
        this->session_manager_ = session_manager;
    }
    proxy_server::proxy_server(const happy_proxy::front_end& frontend,const happy_proxy::proxy_session_manager& session_manager)
    {
       config_ = frontend.operator()();
           this->session_manager_ = new happy_proxy::proxy_session_manager(session_manager);
       
    }
    proxy_server::proxy_server(const happy_proxy::back_end& backend,const happy_proxy::proxy_session_manager& session_manager)
        {
            config_ = backend.operator()();
            this->session_manager_ = new happy_proxy::proxy_session_manager(session_manager);
        }
    
    proxy_server::proxy_server(  server_details& config)
    {
     this->config_ = config; 
     //this->config_ = std::make_unique<ProxyConfig>("config.txt"); 
     this->session_manager_ = new happy_proxy::proxy_session_manager(this);
    }

    proxy_server::~proxy_server()
    {   

    }


    void proxy_server::start()
    {
        //this->session_manager_->start();
        this->initialize();
        
        const size_t nthreads_per_server_  = std::thread::hardware_concurrency();;//default number of  cpu cores
        for (size_t i = 0; i < nthreads_per_server_; i++)
        {
            this->threads_.push_back(std::thread(&proxy_server::run, this));
        }
        std::cout << "Proxy server["<<this->config_.name<<"]"<<" started on port " << std::to_string(this->config_.port)<< std::endl;

    }

    void proxy_server::run()
    {    int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_socket == -1) {
        perror("socket");
        return;
    }

    // Set the socket to allow multiple connections
    int enable = 1;
    if (setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
        close(listen_socket);
        return;
    }

    // Bind the socket to the specified address and port
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(config_.port);
    if (bind(listen_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("bind failed");
        close(listen_socket);
        return;
    }

    // Listen for incoming connections
    if (listen(listen_socket, 3) < 0) {
        perror("listen failed");
        close(listen_socket);
        return;
    }

    // Accept incoming connections
    while (this->running_.load() == true) {
        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        int client_socket = accept(listen_socket, (struct sockaddr *)&client_address, &client_address_len);
        if (client_socket < 0) {
            perror("accept failed");
            continue;
        }

        // Create a new proxy session for the client
        proxy_session* session = new proxy_session(this->session_manager_, this);
        session->set_client_socket(client_socket);
        session->start();

        // Add the session to the session manager
        this->session_manager_->on_create_session(session);
    }

    // Close the listen socket
    close(listen_socket);

    }
    void proxy_server::stop()
    {
     std::cout<<"Stopping proxy server["<<this->config_.name<<"]"<<" on port " << std::to_string(this->config_.port)<< std::endl;
         this->session_manager_->stop();
    }
 
    int proxy_server::get_port()const
    {
        return this->config_.port;
    }
    
    void proxy_server::set_port(int port)
    {
        this->config_.port = port;
    }
    //initialize the proxy server when ready to start
    void proxy_server::initialize()
    {
        this->running_.store(true);
    }

   
} // namespace happy_proxy  
