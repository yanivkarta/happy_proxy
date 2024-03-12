#ifndef HAPPY_PROXY_SESSION_H_
#define HAPPY_PROXY_SESSION_H_

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
#include <iomanip>
#include <chrono>
#include <ctime>
#include <algorithm>


#include "proxy.h"
#include "proxy_util.h"
#include "proxy_config.h"
#include "proxy_session_manager.h"
#include "proxy_server.h"


namespace happy_proxy
{
 
    using namespace happy_proxy;

    class proxy_session
    {
    private:
        proxy_session_manager* manager_ = nullptr;
        proxy_server* server_ = nullptr;
        std::mutex session_mutex_;
        std::atomic<bool> running_;
        //std::vector<std::thread> threads_;
        //session details :
        struct session_details  details_; 
        std::string log_file_;
        std::ofstream session_log_;
        std::mutex log_mutex;
     public:
        explicit proxy_session(happy_proxy::proxy_session_manager* manager, happy_proxy::proxy_server* server); 
        explicit proxy_session(happy_proxy::proxy_session_manager* manager,const  struct happy_proxy::server_details& serverdetails);
        virtual ~proxy_session();
        //copy constructor
        proxy_session(const proxy_session& other);
        //copy assignment
        proxy_session& operator=(const proxy_session& other);
        //move constructor
        proxy_session(proxy_session&& other);
        //move assignment
        proxy_session& operator=(proxy_session&& other);

        // Starts the proxy session. 
        void start();

        // Stops the proxy session. 
        void stop();

        // Returns the proxy session.
        const proxy_session_manager* GetSessionManager()const; 
        
        // Returns the proxy server.
        const proxy_server* GetServer()const;

        // Returns the log file.
        std::string GetLogFile()const;
        
        //get details
        const struct server_details& GetServerDetails()const;
        //set details
        void SetServerDetails(const struct server_details& serverdetails); 
        // Returns the session log.
        std::ofstream GetSessionLog()const;
        //set session log
        void SetSessionLog(const std::ofstream& session_log);
        //set log file
        void SetLogFile(const std::string& log_file);
        //set server
        void SetServer(happy_proxy::proxy_server* server);
        //set session manager
        void SetSessionManager(happy_proxy::proxy_session_manager* manager);
        //set running
        void SetRunning(bool running);
        //set session mutex
        void SetSessionMutex(const std::mutex& session_mutex);
        //set log mutex
        void SetLogMutex(const std::mutex& log_mutex);
        //set session details
        void SetDetails(const struct session_details& sessiondetails); 
        //get session details
        const struct session_details GetDetails()const;
        //initialize the proxy session when ready to start
        virtual void initialize() ;

        virtual void on_create_session(const proxy_session* session); 
        virtual void on_destroy_session(const proxy_session* session); 

        
        //get clients:
        //std::vector<const happy_proxy::proxy_client*> GetClients() const;        
     };

} // namespace happy_proxy

  
#endif