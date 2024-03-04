#include <atomic>
#include "proxy.h"
#include "proxy_config.h"
#include "proxy_util.h"
#include "proxy_server.h"
#include "proxy_client.h"
#include "proxy_session.h"
#include "proxy_session_manager.h"
#include "proxy_acl.h"
#include "proxy_auth.h"
#include "ip_helper.h"

namespace happy_proxy
{
   static NetworkACE<happy_proxy::ip_address> acls;
   static std::mutex acl_mutex;

   //parse acl from line like : acl forbidden_dst url_ip 192.168.0.0/24 
   //in the context of config file ( frontend, backend, etc) 


    std::map<std::string,std::string> parse_acl(const std::string& line)
    {
        std::map<std::string,std::string> acl;
        std::vector<std::string> tokens = happy_proxy::split(line);
        if(tokens.size() != 4)
        {
            throw happy_proxy::exception("Error: invalid acl line");
        }
        acl["type"] = tokens[1];
        acl["name"] = tokens[2];
        acl["value"] = tokens[3];
        return acl;
    }
 

    
};
