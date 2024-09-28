//CTESTS :

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

#include "sampler/sampler.h"
#include "util/hash_helper.h"
#include "load_balancer_strategy/strategy_factory.h"

using namespace happy_proxy;
/// @brief 
/// @return 
int main(int argv,char**argc)
{


    //sampler
    happy_proxy::load_balancer::sampler s;

    std::map<int64_t, std::map<std::string, std::string>> stats = s.get_statistics();
    
    //parse arguments:
    
    s.sample_statistics();
    s.analyze_statistics();

    //hash_helper
    hash_helper<uint64_t> h;
    std::function<uint64_t (const std::string&)> string_hash_function = std::hash<std::string>();
    h.set_string_hash_function(string_hash_function);
    std::function<uint64_t (const std::vector<uint8_t>&)> bytes_hash_function = wang64a;
    h.set_bytes_hash_function(bytes_hash_function);

 //proxy:
     proxy_session_manager psm;
 
    //strategy factory
    load_balancer::strategy_factory sf;
    load_balancer::strategy_factory sf2(sf);
    sf2 = sf;
    load_balancer::strategy_factory sf3(std::move(sf));
    sf3 = std::move(sf2);

    return 0;   

 }   
//end of tests/ctests.cpp