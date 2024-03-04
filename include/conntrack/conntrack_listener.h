#ifndef __CONNTRACK_LISTENER_H_
#define __CONNTRACK_LISTENER_H_
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>

#include <map>

#include <libnfnetlink/libnfnetlink.h>
#include <libnetfilter_conntrack/libnetfilter_conntrack.h>
#include <libnetfilter_conntrack/libnetfilter_conntrack_tcp.h>


#include "conntrack_policy_maker.h"


namespace happy_proxy
{
    namespace conntrack
    {
        class conntrack_listener
        {
        private:
            struct nfct_handle *handle_;
            struct nfct_q_handle *qh_;
            struct nf_conntrack *ct_;
            struct nf_conntrack_tuple *tuple_;
            struct nf_conntrack_tcp *tcp_info_;
            struct nf_conntrack_expect *exp_;
            struct nf_conntrack_helper *helper_;
            struct nf_conntrack_zone *zone_;
        //callbacks
        std::function<void(const std::string &ip, const std::string &port)> add_backend_callback_; 
        std::function<void(const std::string &ip, const std::string &port)> remove_backend_callback_; 
        std::function<void(const std::string &ip, const std::string &port)> add_frontend_callback_; 
        std::function<void(const std::string &ip, const std::string &port)> remove_frontend_callback_; 
        std::function<void(const std::string &src_ip, const std::string &dst_ip, const std::string &src_port, const std::string &dst_port, const std::string &protocol, const std::string &state, const std::string &direction, const std::string &timestamp, const std::string &comments, int timeout, int zone, int id, int mark, int helper, int expect, int mask, int flags, int use, int use_expect, int use_helper, int use_zone)> add_policy_callback_; 
        std::function<void(const std::string &src_ip, const std::string &dst_ip, const std::string &src_port, const std::string &dst_port, const std::string &protocol, const std::string &state, const std::string &direction, const std::string &timestamp, const std::string &comments, int timeout, int zone, int id, int mark, int helper, int expect, int mask, int flags, int use, int use_expect, int use_helper, int use_zone)> remove_policy_callback_; 


        //log
        std::string log_file_;
        std::ofstream conntrack_log_;
        std::mutex log_mutex_;

        //thread
        std::thread *conntrack_thread_;
        std::atomic<bool> conntrack_running_;
        std::mutex conntrack_mutex_;
        std::map<std::string, std::string> backends_;
        std::map<std::string, std::string> frontends_;
        
        //policies
        
        public:
            conntrack_listener(); 
            void add_backend(const std::string& ip, const std::string& port); 
            void remove_backend(const std::string& ip, const std::string& port); 
            void add_frontend(const std::string& ip, const std::string& port); 
            void remove_frontend(const std::string& ip, const std::string& port); 
            void add_policy(const std::string& src_ip, const std::string& dst_ip, const std::string& src_port, const std::string& dst_port, const std::string& protocol, const std::string& state, const std::string& direction, const std::string& timestamp, const std::string& comments, int timeout, int zone, int id, int mark, int helper, int expect, int mask, int flags, int use, int use_expect, int use_helper, int use_zone); 
            void remove_policy(const std::string& src_ip, const std::string& dst_ip, const std::string& src_port, const std::string& dst_port, const std::string& protocol, const std::string& state, const std::string& direction, const std::string& timestamp, const std::string& comments, int timeout, int zone, int id, int mark, int helper, int expect, int mask, int flags, int use, int use_expect, int use_helper, int use_zone); 


            conntrack_listener(const conntrack_listener& other);
            conntrack_listener& operator=(const conntrack_listener& other);
            conntrack_listener(conntrack_listener&& other);
            conntrack_listener& operator=(conntrack_listener&& other);
            virtual ~conntrack_listener();
            virtual void initialize();
            //callbacks : 
            void add_backend_callback( std::function<void(const std::string &ip, const std::string &port)> callback); 
            void remove_backend_callback( std::function<void(const std::string &ip, const std::string &port)> callback); 
            void add_frontend_callback( std::function<void(const std::string &ip, const std::string &port)> callback); 
            void remove_frontend_callback( std::function<void(const std::string &ip, const std::string &port)> callback); 
            void add_policy_callback( std::function<void(const std::string &src_ip, const std::string &dst_ip, const std::string &src_port, const std::string &dst_port, const std::string &protocol, const std::string &state, const std::string &direction, const std::string &timestamp, const std::string &comments, int timeout, int zone, int id, int mark, int helper, int expect, int mask, int flags, int use, int use_expect, int use_helper, int use_zone)> callback); 
            void remove_policy_callback( std::function<void(const std::string &src_ip, const std::string &dst_ip, const std::string &src_port, const std::string &dst_port, const std::string &protocol, const std::string &state, const std::string &direction, const std::string &timestamp, const std::string &comments, int timeout, int zone, int id, int mark, int helper, int expect, int mask, int flags, int use, int use_expect, int use_helper, int use_zone)> callback); 
            void Log(const std::string& message ); 

            virtual void finalize();



            void Log(const std::string& message );


        };
    }//conntrack
} //happy_proxy 

#endif
