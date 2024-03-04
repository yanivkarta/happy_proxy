#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <mutex>

//strategy factory for load balancer:
#include "load_balancer_strategy/strategy_factory.h"
//proxy server:
#include "proxy_server.h"
//proxy configuration:
#include "proxy_config.h"
//proxy session manager:
#include "proxy_session_manager.h"
//proxy utility:
#include "proxy_util.h"


namespace happy_proxy
{
    namespace load_balancer{
    //
    //strategy factory implementation: 
    //

    //constructor
    strategy_factory::strategy_factory()
    {
        this->log_file_ = "strategy_log.txt";
        this->strategy_log_.open(this->log_file_, std::ios::out);
    }

    //copy constructor
    strategy_factory::strategy_factory(const strategy_factory& other)
    {
        this->strategies_ = other.strategies_;
        this->log_file_ = other.log_file_;
        this->strategy_log_.open(this->log_file_, std::ios::out);
    }   

    //copy assignment operator  
    strategy_factory& strategy_factory::operator=(const strategy_factory& other)
    {
        if(this != &other)
        {
            this->strategies_ = other.strategies_;
            this->log_file_ = other.log_file_;
            this->strategy_log_.open(this->log_file_, std::ios::out);
        }
        return *this;
    }    
    //move constructor
    strategy_factory::strategy_factory(strategy_factory&& other)
    {
        this->strategies_ = std::move(other.strategies_);
        this->log_file_ = std::move(other.log_file_);
        this->strategy_log_.open(this->log_file_, std::ios::out);   

    }
    //move assignment operator
    strategy_factory& strategy_factory::operator=(strategy_factory&& other)
    {
        if(this != &other)
        {
            this->strategies_ = std::move(other.strategies_);
            this->log_file_ = std::move(other.log_file_);
            this->strategy_log_.open(this->log_file_, std::ios::out);
        }
        return *this;
    }
    //destructor
    strategy_factory::~strategy_factory()
    {
        this->strategy_log_.close();
    }   
    //add strategy
    void strategy_factory::add_strategy(std::string name, std::unique_ptr<strategy> strategy)
    {
        std::lock_guard<std::mutex> lock(this->strategy_mutex_);
        this->strategies_[name] = strategy.release();
    }   
    //remove strategy   
    void strategy_factory::remove_strategy(std::string name)
    {
        std::lock_guard<std::mutex> lock(this->strategy_mutex_);
        auto it = this->strategies_.find(name);
        if(it != this->strategies_.end())
        {
            delete it->second;
            this->strategies_.erase(it);
        }
    }   
    //get strategy
    std::unique_ptr<strategy> strategy_factory::get_strategy(std::string name)
    {
        std::lock_guard<std::mutex> lock(this->strategy_mutex_);
        auto it = this->strategies_.find(name);
        if(it != this->strategies_.end())
        {
            return std::unique_ptr<strategy>(it->second);
        }
        return std::unique_ptr<strategy>(nullptr);
    }   
    //log
    void strategy_factory::Log(const std::string& message)
    {
        std::lock_guard<std::mutex> lock(this->log_mutex_);
        this->strategy_log_ << message << std::endl;
    }
    }//load_balancer    
} //happy_proxy


//end of file
