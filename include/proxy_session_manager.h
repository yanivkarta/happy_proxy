#ifndef HAPPY_PROXY_SESSION_MANAGER_H   
#define HAPPY_PROXY_SESSION_MANAGER_H




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



#include "proxy_util.h"
#include "proxy_session.h"
#include "proxy_server.h"
#include "proxy_client.h"



namespace happy_proxy
{
    class proxy_server;
    class proxy_session;
    
    class proxy_session_manager
    {
    protected:
        proxy_server* server_ = nullptr;
        std::vector<const proxy_session *> sessions_;
        std::mutex session_mutex_;
        std::atomic<bool> running_;
        std::vector<std::thread> threads_ ;
        std::ofstream session_log_; //append to this file 
                                    //when a session is created or destroyed 
        std::mutex log_mutex_;
        std::string log_file_;
       public:
        proxy_session_manager();
        proxy_session_manager(happy_proxy::proxy_server* server); 
        proxy_session_manager(struct server_details &server_config); 
        proxy_session_manager(const proxy_session_manager& other);
        proxy_session_manager& operator=(const proxy_session_manager& other); 
        proxy_session_manager(proxy_session_manager&& other);
        proxy_session_manager& operator=(proxy_session_manager&& other); 


        virtual ~proxy_session_manager();

        // Starts the proxy session manager. 
        virtual void start();

        // Stops the proxy session manager. 
        virtual void stop();

        // Returns the proxy session manager.
        // synchronized
        virtual void on_create_session(const proxy_session* session); 
        virtual void on_destroy_session(const proxy_session* session); 
        const proxy_server* GetServer()const; 
        const std::vector<const proxy_session *> GetSessions()const; 
        const std::string GetLogFile()const;
        void SetLogFile(const std::string& log_file);
        void SetServer(happy_proxy::proxy_server* server);
        void SetSessions(const std::vector<const proxy_session *>& sessions);
        void SetRunning(bool running);
        void SetThreads(const std::vector<std::thread>& threads);
         void SetSessionLog(const std::ofstream& session_log);

    }; 


    class default_proxy_session_manager : public proxy_session_manager
    { 
    public:
        default_proxy_session_manager();
        default_proxy_session_manager(happy_proxy::proxy_server* server);
        virtual ~default_proxy_session_manager();
        // Starts the proxy session manager. 
        void start() override;

        // Stops the proxy session manager. 
        void stop() override;

        // Returns the proxy session manager.
        // synchronized
        void on_create_session(const proxy_session* session) override; 
        void on_destroy_session(const proxy_session* session) override; 
    };  

}   // namespace happy_proxy

#endif // HAPPY_PROXY_SESSION_MANAGER_H