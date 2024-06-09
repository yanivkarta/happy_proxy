#ifndef ROUTE_POLICY_H_
#define ROUTE_POLICY_H_

#include <string>
#include <vector>
#include <map>
#include <set>

#include "proxy_util.h"

//resource route policy: 

#include "load_balancer_strategy/strategy.h"
#include "load_balancer_strategy/strategy_factory.h"
#include "load_balancer_strategy/server_stats.h"
#include "load_balancer_strategy/resource.h"


namespace happy_proxy
{
    namespace load_balancer
    {
        class route_policy
        {
            public:
                route_policy();
                virtual ~route_policy();
                virtual std::vector<resource> get_route_policy(std::vector<resource> resources, std::vector<server_stats> server_stats) = 0;
        };
    }   
    //namespace load_balancer
}




#endif