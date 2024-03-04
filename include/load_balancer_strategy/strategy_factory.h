#ifndef __STRATEGY_FACTORY_H_
#define __STRATEGY_FACTORY_H_



#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>
#include <fstream>
#include <iostream>
#include <sstream>
//unique_ptr
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>

#include "strategy.h"
#include "proxy_util.h"
#include "proxy_config.h"
#include "proxy_session_manager.h"
#include "proxy_server.h"

namespace happy_proxy
{
    namespace load_balancer
    {
    class strategy_factory
    {
    private:
        //std::map<std::string, std::unique_ptr<strategy>> strategies_;
        std::map<std::string, strategy*> strategies_;
        std::mutex strategy_mutex_;
        std::string log_file_;
        std::ofstream strategy_log_;
        std::mutex log_mutex_;
    public:
        strategy_factory();
        strategy_factory(const strategy_factory& other);
        strategy_factory& operator=(const strategy_factory& other);
        strategy_factory(strategy_factory&& other);
        strategy_factory& operator=(strategy_factory&& other);
        virtual ~strategy_factory();
        void add_strategy(std::string name, std::unique_ptr<strategy> strategy);
        void remove_strategy(std::string name);
        std::unique_ptr<strategy> get_strategy(std::string name);
        void Log(const std::string& message);
    };
    }//load_balancer
} //happy_proxy

#endif
