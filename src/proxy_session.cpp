#include "proxy.h"
#include "proxy_config.h"
#include "proxy_session_manager.h"
#include "proxy_server.h"
#include "proxy_session.h"

namespace happy_proxy
{
    //proxy_session implementation
    proxy_session::proxy_session(happy_proxy::proxy_session_manager* manager, happy_proxy::proxy_server* server)
    {
        this->manager_ = manager;
        this->server_ = server;
    }

    proxy_session::~proxy_session()
    {
        //delete this->server_;
    }   
    void proxy_session::Start()
    {
        this->server_->Start();
    }
    void proxy_session::Stop()
    {
        this->server_->Stop();
    }
    const proxy_server* proxy_session::GetServer()const
    {
        return this->server_;
    }
    const struct session_details proxy_session::GetDetails()const
    {
        return this->details_;
    }
    const proxy_session_manager* proxy_session::GetSessionManager()const
    {
        return this->manager_;
    }
    void proxy_session::SetServer(proxy_server* server)
    {
        this->server_ = server;
    }
    void proxy_session::SetDetails(const struct session_details& details)
    {
        this->details_ = details;
    }
    void proxy_session::SetSessionManager(proxy_session_manager* manager)
    {
        this->manager_ = manager;
    }
    void proxy_session::on_create_session(const proxy_session* session)
    {
        this->manager_->on_create_session(session);
    }
    void proxy_session::on_destroy_session(const proxy_session* session)
    {
        this->manager_->on_destroy_session(session);
    }
    //initialize the proxy session when ready to start
    void proxy_session::Initialize()
    {
        //initialize the proxy session when ready to start
    }

    
  
  

}