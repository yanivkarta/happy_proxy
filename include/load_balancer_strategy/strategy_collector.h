#ifndef __STRATEGY_COLLECTOR_H_
#define __STRATEGY_COLLECTOR_H_


#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>
#include <fstream>


#include "strategy.h"
#include "strategy_factory.h"
#include "strategy_collector.h"

//for ifla statistics from libnl
#include <linux/if_link.h>  //IFLA_STATS64

namespace happy_proxy 
{
    
    namespace load_balancer
    {
    //strategy collector: 
    class strategy_collector
    {
    private: 
    //strategies:
        std::vector<std::shared_ptr<load_balancer::strategy>> strategies_; 
        std::mutex strategy_mutex_;
        std::atomic<bool> running_;
        std::vector<std::thread> threads_;
        std::ofstream strategy_log_;
        std::mutex log_mutex_;
        std::string log_file_;      

    public:
    //add remove strategies to collection; 
        void add_strategy(const std::shared_ptr<load_balancer::strategy>& strategy); 
        void remove_strategy(const std::shared_ptr<load_balancer::strategy>& strategy); 
        void update_strategy(const std::shared_ptr<load_balancer::strategy>& strategy); 
        void apply_strategies(); 
        
        void Start(); 
        void Stop(); 
        
        //thread function to collect with thisptr as argument:
        void Collect( strategy_collector* collector);

        //log message:
        void Log(const std::string& message); 
        //copy constructor
        strategy_collector(const strategy_collector& other);
        //copy assignment
        strategy_collector& operator=(const strategy_collector& other);
        //move constructor
        strategy_collector(strategy_collector&& other);
        //move assignment
        strategy_collector& operator=(strategy_collector&& other);
        //destructor
        virtual ~strategy_collector();
        

    };
    }

}


#endif 
