#ifndef __CONNECTION_H_
#define __CONNECTION_H_


#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <chrono>
//netfilter conntrack
#include <libnfnetlink/libnfnetlink.h>  
#include <libnetfilter_conntrack/libnetfilter_conntrack.h> 
#include <libnetfilter_conntrack/libnetfilter_conntrack_tcp.h> 


namespace happy_proxy
{
    namespace conntrack
    {
        struct connection_details
        {
            
            std::string src_ip;
            std::string dst_ip;
            std::string src_port;
            std::string dst_port;
            std::string protocol;
            std::string state;
            std::string direction;
            std::string timestamp;
            std::string comments;
            int timeout;
            int zone;
            int id;
            int mark;
            int helper;
            int expect;
            int mask;
            int flags;
            int use;
            int use_expect;
            int use_helper;
            int use_zone;



        };
        class connection
        {
        protected:
            connection_details details_;
            struct nfct_handle *handle_;
            struct nfct_q_handle *qh_;
            struct nf_conntrack *ct_;
            struct nf_conntrack_tuple *tuple_;
            struct nf_conntrack_tcp *tcp_info_;
            struct nf_conntrack_expect *exp_;
            struct nf_conntrack_helper *helper_;
            struct nf_conntrack_zone *zone_;

        public:
            connection(const connection_details &details) : details_(details) {}
            connection(const connection &other);
            connection &operator=(const connection &other);
            connection(connection &&other);
            connection &operator=(connection &&other);
            virtual ~connection() = default;
            void add_backend_callback( std::function<void(const std::string &ip, const std::string &port)> callback); 
            void remove_backend_callback( std::function<void(const std::string &ip, const std::string &port)> callback);
            void add_frontend_callback( std::function<void(const std::string &ip, const std::string &port)> callback);
            void remove_frontend_callback( std::function<void(const std::string &ip, const std::string &port)> callback);
            void add_policy_callback( std::function<void(const std::string &src_ip, const std::string &dst_ip, const std::string &src_port, const std::string &dst_port, const std::string &protocol, const std::string &state, const std::string &direction, const std::string &timestamp, const std::string &comments, int timeout, int zone, int id, int mark, int helper, int expect, int mask, int flags, int use, int use_expect, int use_helper, int use_zone)> callback);
            void remove_policy_callback( std::function<void(const std::string &src_ip, const std::string &dst_ip, const std::string &src_port, const std::string &dst_port, const std::string &protocol, const std::string &state, const std::string &direction, const std::string &timestamp, const std::string &comments, int timeout, int zone, int id, int mark, int helper, int expect, int mask, int flags, int use, int use_expect, int use_helper, int use_zone)> callback);
 
        };  
    } // namespace conntrack
} // namespace happy_proxy
#endif
             
