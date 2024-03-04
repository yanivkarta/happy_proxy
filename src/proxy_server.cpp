

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
    proxy_server::proxy_server(struct server_details& config)
    {
     this->config_ = config; 
     //this->config_ = std::make_unique<ProxyConfig>("config.txt"); 
     this->session_manager_ = new happy_proxy::proxy_session_manager(this);

      
    }

    proxy_server::~proxy_server()
    {   

    }


    void proxy_server::Start()
    {
        this->session_manager_->Start();
        this->Initialize();
    }

    void proxy_server::Stop()
    {
         this->session_manager_->Stop();
    }
 
    int proxy_server::GetPort()
    {
        return this->config_.port;
    }
    
    void proxy_server::SetPort(int port)
    {
        this->config_.port = port;
    }
    //initialize the proxy server when ready to start
    void proxy_server::Initialize()
    {
        //initialize the proxy server when ready to start
    }

   
} // namespace happy_proxy  
