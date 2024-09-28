#include <atomic>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <mutex>
#include <thread>
#include <fstream>
#include <functional>
#include <iomanip>
#include <sstream>
#include <syslog.h>
#include <sys/types.h>

#include "proxy.h"
#include "load_balancer_strategy/strategy.h"
 #include "util/hash_helper.h"

namespace happy_proxy
{
    namespace load_balancer{

    //strategy class 
    //copy constructor according to definition order
    strategy::strategy(const strategy& other): details_(other.details_),servers_(other.servers_),_sticky_session_(false) 
    {
        //copy constructor  
    }
     //strategy class
    strategy::~strategy() {}
    //round robin strategy
    //round_robin_strategy::~round_robin_strategy()  {}
    //copy constructor
    round_robin_strategy::round_robin_strategy(const round_robin_strategy& other):strategy(other),current_index_(other.current_index_), servers_(other.servers_) {}
    
    //copy assignment operator
    round_robin_strategy& round_robin_strategy::operator=(const round_robin_strategy& other)
    {
        if (this != &other)
        {
            servers_ = other.servers_;
            current_index_ = other.current_index_;
        }
        return *this;
    }
    //move constructor
    round_robin_strategy::round_robin_strategy(round_robin_strategy&& other):strategy(other),current_index_(std::move(other.current_index_)), servers_(std::move(other.servers_)) {} 
    
    //move assignment operator
    round_robin_strategy& round_robin_strategy::operator=(round_robin_strategy&& other)
    {
        if (this != &other)
        {
            servers_ = std::move(other.servers_);
            current_index_ = std::move(other.current_index_);
        }
        return *this;
    }
    //destructor
    round_robin_strategy::~round_robin_strategy() {}

    std::string round_robin_strategy::get_next_server()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (servers_.empty())
        {
            throw exception("No servers available");
        }
        if (current_index_ >= servers_.size())
        {
            current_index_ = 0;
        }
        return servers_[current_index_++].name;
    }

    void strategy::add_server(const server_details& server)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        //add server to the list of servers

        servers_.push_back(server);
        

    }   
    
    std::string random_strategy::get_next_server()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (servers_.empty())
        {
            throw exception("No servers available");
        }
        std::uniform_int_distribution<size_t> distribution(0, servers_.size() - 1);
        return servers_[0].name;
    } 
    strategy::strategy():_sticky_session_(false) {}

    //random strategy default constructor 
    random_strategy::random_strategy():strategy() {}

    //least connection strategy
    //least_connection_strategy::least_connection_strategy() {}
    least_connection_strategy::~least_connection_strategy() {}

    std::string least_connection_strategy::get_next_server()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (servers_.empty())
        {
            throw exception("No servers available");
        }
        if (server_statistics_updated_)
        {
            std::lock_guard<std::mutex> lock(server_statistics_mutex_);
            server_statistics_updated_ = false;
            std::string next = servers_[0].name;
            return next;
            
        }
        return "";
    }   
 
    //performance strategy
 
    performance_strategy::~performance_strategy()
    {
        server_statistics_updater_.join();
    }   
    std::string performance_strategy::get_next_server()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (servers_.empty())
        {
            throw exception("No servers available");
        }
        if (server_statistics_updated_)
        {
            std::lock_guard<std::mutex> lock(server_statistics_mutex_);
            server_statistics_updated_ = false;
            std::string next = servers_[0].name;
            return next;
        }
        return ""; 
    }
    void strategy::remove_server( const server_details& server)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        servers_.erase(std::remove(servers_.begin(), servers_.end(), server), servers_.end());
    }   
    void strategy::update_server( const server_details& server)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = std::find(servers_.begin(), servers_.end(), server); 
        if (it != servers_.end())
        {
            *it = server;
        }

        
    }
    strategy_ensemble::strategy_ensemble(const strategy_ensemble& other)
    {
        for (auto& st : other.strategies_)
        {
            strategies_.push_back(st->clone());
        }
        weights_ = other.weights_;
    } 

    strategy_ensemble& strategy_ensemble::operator=(const strategy_ensemble& other)
    {
        if (this != &other)
        {
            for (auto& st : other.strategies_)
            {
                strategies_.push_back(st->clone());



            }   
 
            //copy the strategies
        }
        return *this;
    }
    strategy_ensemble::strategy_ensemble(strategy_ensemble&& other) 
    {
        if (this != &other)
        {
            strategies_ = std::move(other.strategies_);
            weights_ = std::move(other.weights_);
            //move the strategies
        }   
    }
    strategy_ensemble& strategy_ensemble::operator=(strategy_ensemble&& other)
    {
        if (this != &other)
        {
           strategies_ = std::move(other.strategies_); 
           //move the strategies     

        }
        return *this; 
    }

    //constructor   
    strategy_ensemble::strategy_ensemble() {}
    //destructor
    strategy_ensemble::~strategy_ensemble()
    {
        //lock mutex
        std::lock_guard<std::mutex> lock(mutex_); 

        for (auto& st : strategies_)
        {
            delete st;
        }
    }
    //add strategy


    //strategy clone:
    strategy* round_robin_strategy::clone()
    {
        return new round_robin_strategy(*this);
    }
    strategy* random_strategy::clone()
    {
        return  new random_strategy;//nothing to clone
    }
    strategy* least_connection_strategy::clone()
    {
        return new least_connection_strategy(*this);    
    }
    strategy* performance_strategy::clone()
    {
        return new performance_strategy(*this); 
    }
    strategy_ensemble* strategy_ensemble::clone()
    {
        return new strategy_ensemble(*this);    
    }

    //apply:
    void performance_strategy::apply()
    {
        //already applied... start the server statistics updater thread if not already started
        if (!server_statistics_updater_.joinable())
        {
            server_statistics_updater_ = std::thread(&performance_strategy::update_server_statistics, this);
            //start the server statistics updater thread if not already started

        }

    }
    //RR apply: 
    void round_robin_strategy::apply()
    {
        //nothing to apply
    }
    

    //random apply
    void random_strategy::apply()
    {
        //nothing to apply
    }
    //least connection apply
    void least_connection_strategy::apply()
    {
        //nothing to apply
    }
    //ensemble apply

    void strategy_ensemble::apply()
    {
        for (auto& st : strategies_)
        {
            st->apply();
        }
    }   
    //performance strategy copy constructor:
    performance_strategy::performance_strategy(const performance_strategy& other):strategy(other), server_statistics_(other.server_statistics_), server_statistics_updated_(false) {} 
    

    //update server statistics
    void performance_strategy::update_server_statistics()
    {
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            std::lock_guard<std::mutex> lock(server_statistics_mutex_);
            server_statistics_updated_ = true;
            //collect server statistics
            
        }
    }


    //least connection copy constructor
    least_connection_strategy::least_connection_strategy(const least_connection_strategy& other):strategy(other), server_statistics_(other.server_statistics_), server_statistics_updated_(false) {} 
    //least connection move constructor
    //update/remove servers

    /*
    void least_connection_strategy::update_server(const server_details& server)
    {
        std::lock_guard<std::mutex> lock(server_statistics_mutex_);
        
        happy_proxy::server_id_hash hasher(server.name,server.ip_address,server.port); 
        auto server_id = hasher.hash_server_id();

        auto it = std::find_if(server_statistics_.begin(), server_statistics_.end(), [server_id](const happy_proxy::load_balancer::strategy_server_statistics& s) { return s.server_id == server_id; }); 

        if (it != server_statistics_.end())
        {
            //update the server statistics 
            server_statistics_updated_ = true;
        }   
    }
    void least_connection_strategy::remove_server(const server_details& server)
    {
        std::lock_guard<std::mutex> lock(server_statistics_mutex_);
        //remove the server statistics of the server id: 
        //find the server entry in statistics and remove it 
        auto name = server.name;
        //hash the server id : 
        happy_proxy::server_id_hash hasher(server.name,server.ip_address,server.port);
        auto server_id = hasher.hash_server_id();
        auto it = std::find_if(server_statistics_.begin(), server_statistics_.end(), [server_id](const happy_proxy::load_balancer::strategy_server_statistics& s) { return s.server_id == server_id; }); 
        if (it != server_statistics_.end())
        {
            server_statistics_.erase(it);
            server_statistics_updated_ = true;
        }
    }
    */
    }//load_balancer

   
} // namespace happy_proxy

// Path: src/strategies/strategy_factory.cpp
