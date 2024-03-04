#ifndef HAPPY_PROXY_CONFIG_H
#define HAPPY_PROXY_CONFIG_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>
 
#include "proxy.h"

namespace happy_proxy
{
    class proxy_config
    {
    private:
        std::string config_file_path_;
        std::vector<std::string> config_file_paths_; 
        //we don't declare the proxy server, client and session manager here. 
        //we will create them in the Initialize() method. 
        
        happy_proxy::ProxyConfigContainerPtr config_;
        std::mutex config_mutex_;
        


   public:
        proxy_config( const std::string& config_file_path = "happy_proxy.cfg");
        
        ~proxy_config();

        // Loads the configurations from local or service/daemon and initializes the proxy. 
        // This is a blocking call and will not return until the proxy is initialized. 
        void Initialize();
        // Returns the proxy session manager. 
        std::string ParseSectionLine(const std::vector<std::string>& sections , const std::string& line) ;

        ProxyConfigContainer* GetConfig()const; 
        //get frontends 
        std::vector<front_end> get_frontends() const;
        //get backends
        std::vector<back_end>  get_backends() const;
        //get server details
        std::vector<server_details> get_servers_details() const;
        

        //get globals returns config_->globals:
        const struct  global_parameters&
        get_globals() const;

        

    };
}

#endif