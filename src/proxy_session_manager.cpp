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

namespace happy_proxy
{

proxy_session_manager::proxy_session_manager()
{
    this->running_ = false;
    this->log_file_ = "session.log";
    this->session_log_.open(this->log_file_, std::ios::app);
}
proxy_session_manager::proxy_session_manager(happy_proxy::proxy_server* server)
{
    this->server_ = server;
    this->running_ = false;
    this->log_file_ = "session.log";
    this->session_log_.open(this->log_file_, std::ios::app); 
}

proxy_session_manager::~proxy_session_manager()
{
    this->session_log_.close(); 
}

void proxy_session_manager::Start()
{
    this->running_ = true;
    this->session_log_ << "Session Manager Started" << std::endl;
    this->session_log_.flush();
    this->session_log_.close();     
}
// Stops the proxy session manager.
void proxy_session_manager::Stop()
{
    this->running_ = false;
    this->session_log_ << "Session Manager Stopped" << std::endl;
    this->session_log_.flush();
    this->session_log_.close(); 
}   
//copy constructor
proxy_session_manager::proxy_session_manager(const proxy_session_manager& other)
{
    this->server_ = other.server_;
    this->running_ = (bool) other.running_;
    this->log_file_ = other.log_file_;
    this->session_log_.open(this->log_file_, std::ios::app);
    this->session_log_ << "Session Manager Copied" << std::endl;
    this->session_log_.flush();
    this->session_log_.close();     
}
//copy assignment
proxy_session_manager& proxy_session_manager::operator=(const proxy_session_manager& other)
{
    this->server_ = other.server_;
    this->running_ = (bool)other.running_;
    this->log_file_ = other.log_file_;
    this->session_log_.open(this->log_file_, std::ios::app);
    this->session_log_ << "Session Manager Assigned" << std::endl;
    this->session_log_.flush();
    this->session_log_.close();     
    return *this;   
}
//move constructor
proxy_session_manager::proxy_session_manager(proxy_session_manager&& other)
{
    this->server_ = other.server_;
    this->running_ = (bool)other.running_;
    this->log_file_ = other.log_file_;
    this->session_log_.open(this->log_file_, std::ios::app);
    this->session_log_ << "Session Manager Moved" << std::endl;
    this->session_log_.flush();
    this->session_log_.close();         
}
//move assignment
proxy_session_manager& proxy_session_manager::operator=(proxy_session_manager&& other)
{
    this->server_ = other.server_;
    this->running_ = (bool)other.running_;
    this->log_file_ = other.log_file_;
    this->session_log_.open(this->log_file_, std::ios::app);
    this->session_log_ << "Session Manager Assigned" << std::endl;
    this->session_log_.flush();
    this->session_log_.close();     
    return *this;   
}   
//on create/on destroy session
void proxy_session_manager::on_create_session(const proxy_session* session)
{
    this->session_mutex_.lock();
    this->sessions_.push_back(session);
    this->session_mutex_.unlock();
    this->session_log_.open(this->log_file_, std::ios::app);
    this->session_log_ << "Session Created" << std::endl;
    this->session_log_.flush();
    this->session_log_.close();     
}
void proxy_session_manager::on_destroy_session(const proxy_session* session)
{
    this->session_mutex_.lock();
    this->sessions_.erase(std::remove(this->sessions_.begin(), this->sessions_.end(), session), this->sessions_.end());
    this->session_mutex_.unlock();
    this->session_log_.open(this->log_file_, std::ios::app);
    this->session_log_ << "Session Destroyed" << std::endl;
    this->session_log_.flush();
    this->session_log_.close();             
}



}// namespace happy_proxy    


