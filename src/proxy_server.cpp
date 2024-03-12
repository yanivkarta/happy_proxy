

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
    {
       do
       {
            //handle input from clients
            //handle output to clients
            //handle input from servers
            //handle output to servers
            std::cout << "." << std::endl;

       } while (this->running_.load() == true);
       
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
        //initialize the proxy server
    }

   
} // namespace happy_proxy  
