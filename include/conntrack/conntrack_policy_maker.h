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

#include <linux/netfilter/nf_conntrack_common.h>
#include <linux/netfilter/nf_conntrack_tuple_common.h>
#include <linux/netfilter/nf_conntrack_tcp.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/nf_conntrack_sctp.h>
#include <linux/netfilter/xt_conntrack.h>
#include <linux/netfilter/xt_helper.h>
#include <linux/netfilter/xt_conntrack.h>
#include <linux/netfilter/xt_iprange.h>
#include <linux/netfilter/xt_rpfilter.h>
#include <linux/netfilter/xt_connmark.h>

#include <linux/netfilter/xt_connbytes.h>
#include <linux/netfilter/xt_connlimit.h>
#include <linux/netfilter/xt_conntrack.h>
#include <linux/netfilter/xt_connlabel.h>

//for nfct_handle:
extern "C" {
    namespace {

        //make sure tcp enum is not redefined. 
        //define override the preprocessor:
        #define _LIBNETFILTER_CONNTRACK_TCP_H_
        #include <libnetfilter_conntrack/libnetfilter_conntrack.h>
        #undef _LIBNETFILTER_CONNTRACK_TCP_H_
        typedef struct nfct_handle nfct_handle;

         
        

    }
}
namespace happy_proxy
{
    namespace conntrack
    {
        struct nfct_q_handle
        {
            int fd;
            int id;
            int queue_num;
            int copy_range;
            int copy_mode;
            int flags;
            int maxlen;
            int queue_maxlen;

            
            };  
        struct connection_details
        {
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


            std::string src_ip;
            std::string dst_ip;
            std::string src_port;
            std::string dst_port;
            std::string protocol;
            std::string state;
            std::string direction;
            std::string timestamp;
            std::string comments;
            
            inline bool operator == ( const connection_details &other) const
            {
                return (src_ip == other.src_ip && dst_ip == other.dst_ip && src_port == other.src_port && dst_port == other.dst_port && protocol == other.protocol && state == other.state && direction == other.direction && timestamp == other.timestamp && comments == other.comments && timeout == other.timeout && zone == other.zone && id == other.id && mark == other.mark && helper == other.helper && expect == other.expect && mask == other.mask && flags == other.flags && use == other.use && use_expect == other.use_expect && use_helper == other.use_helper && use_zone == other.use_zone);
            }
            inline bool operator != ( const connection_details &other) const
            {
                return !(*this == other);
            }

            connection_details() = default;
            connection_details(const connection_details &other) = default; 
            connection_details &operator=(const connection_details &other) = default; 
            connection_details(connection_details &&other) = default;
            connection_details &operator=(connection_details &&other) = default;
            ~connection_details() = default;
        };
        class connection
        {  
         protected:
             connection_details details_;
             void *handle_;
             void *qh_;
             void *ct_;
             void *tuple_;
             void *tcp_info_;
             void *exp_;
             void *helper_;
             void *zone_;
                std::vector<std::function<void(const std::string &ip, const std::string &port)>> backend_callbacks_;    
                std::vector<std::function<void(const std::string &ip, const std::string &port)>> frontend_callbacks_;
                std::vector<std::function<void(const std::string &src_ip, const std::string &dst_ip, const std::string &src_port, const std::string &dst_port, const std::string &protocol, const std::string &state, const std::string &direction, const std::string &timestamp, const std::string &comments, int timeout, int zone, int id, int mark, int helper, int expect, int mask, int flags, int use, int use_expect, int use_helper, int use_zone)>> policy_callbacks_; 
                std::mutex callback_mutex_;
                std::mutex conntrack_mutex_;
                std::map<std::string, std::string> backends_;
                std::map<std::string, std::string> frontends_;
                std::string log_file_;
                std::ofstream conntrack_log_;

        public:
            connection(const connection_details &details);
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
             
