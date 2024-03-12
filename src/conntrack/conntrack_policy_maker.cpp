#include "conntrack/conntrack_policy_maker.h"
#include <libnetfilter_conntrack/libnetfilter_conntrack.h>
 #include <libnfnetlink/libnfnetlink.h>
#include <linux/netfilter/nf_conntrack_common.h>
#include <linux/netfilter/nf_conntrack_tuple_common.h>
#include <linux/netfilter/nf_conntrack_tcp.h>
#include <linux/netfilter/nf_conntrack_sctp.h>
#include <linux/netfilter/xt_conntrack.h>
#include <linux/netfilter/xt_helper.h>
#include <linux/netfilter/xt_iprange.h>
#include <linux/netfilter/xt_rpfilter.h>
#include <linux/netfilter/xt_connmark.h>



namespace happy_proxy
{ 
    namespace conntrack
    {

        //happy_proxy::conntrack::connection implementation : 
              
        
        //constructor:

        //copy constructor:
        connection::connection(const connection& other)
        {
            //initialize the connection details:
            handle_=qh_=ct_=tuple_=tcp_info_=exp_=helper_=zone_=nullptr;
            

            details_.src_ip = other.details_.src_ip;
            details_.dst_ip = other.details_.dst_ip;
            details_.src_port = other.details_.src_port;
            details_.dst_port = other.details_.dst_port;
            details_.protocol = other.details_.protocol;
            details_.state = other.details_.state;
            details_.direction = other.details_.direction;
            
            details_.timeout = other.details_.timeout;  
            details_.zone = other.details_.zone;
            details_.id = other.details_.id;
            details_.mark = other.details_.mark;
            details_.helper = other.details_.helper;
            details_.expect = other.details_.expect;
            details_.mask = other.details_.mask;
            details_.flags = other.details_.flags;
            details_.use = other.details_.use;
            details_.use_expect = other.details_.use_expect;
            details_.use_helper = other.details_.use_helper;
            details_.use_zone = other.details_.use_zone;
        }   
        //connection_details constructor:
        connection::connection(const connection_details& details):details_(details)
        {
            //initialize the conntrack handle
            handle_ = nfct_open(CONNTRACK, 0);
            if (handle_ == nullptr)
            {
                //log error
                throw std::runtime_error("nfct_open failed");
            }
            //initialize the conntrack queue handle
           
        }
        //add remove policy callbacks   :
        /*            void add_backend_callback( std::function<void(const std::string &ip, const std::string &port)> callback); 
            void remove_backend_callback( std::function<void(const std::string &ip, const std::string &port)> callback);
            void add_frontend_callback( std::function<void(const std::string &ip, const std::string &port)> callback);
            void remove_frontend_callback( std::function<void(const std::string &ip, const std::string &port)> callback);
            void add_policy_callback( std::function<void(const std::string &src_ip, const std::string &dst_ip, const std::string &src_port, const std::string &dst_port, const std::string &protocol, const std::string &state, const std::string &direction, const std::string &timestamp, const std::string &comments, int timeout, int zone, int id, int mark, int helper, int expect, int mask, int flags, int use, int use_expect, int use_helper, int use_zone)> callback);
            void remove_policy_callback( std::function<void(const std::string &src_ip, const std::string &dst_ip, const std::string &src_port, const std::string &dst_port, const std::string &protocol, const std::string &state, const std::string &direction, const std::string &timestamp, const std::string &comments, int timeout, int zone, int id, int mark, int helper, int expect, int mask, int flags, int use, int use_expect, int use_helper, int use_zone)> callback);
        */
        void connection::add_backend_callback( std::function<void(const std::string &ip, const std::string &port)> callback)
        {
            backend_callbacks_.push_back(callback);
        }   
        void connection::remove_backend_callback( std::function<void(const std::string &ip, const std::string &port)> callback)
        {
             //std::find callback in backend callbacks and remove it
            for ( auto it = backend_callbacks_.begin(); it != backend_callbacks_.end(); ++it)
            {
                if (&(*it) == &callback)
                {
                    backend_callbacks_.erase(it);
                    break;
                }
            } 
        }
        void connection::add_frontend_callback( std::function<void(const std::string &ip, const std::string &port)> callback)
        {
            frontend_callbacks_.push_back(callback);
        }
        void connection::remove_frontend_callback( std::function<void(const std::string &ip, const std::string &port)> callback)
        {
            //std::find callback in frontend callbacks and remove it
            for ( auto it = frontend_callbacks_.begin(); it != frontend_callbacks_.end(); ++it)
            {
                if (&(*it) == &callback)
                {
                    frontend_callbacks_.erase(it);
                    break;
                }
            }   

        }
        void connection::add_policy_callback( std::function<void(const std::string &src_ip, const std::string &dst_ip, const std::string &src_port, const std::string &dst_port, const std::string &protocol, const std::string &state, const std::string &direction, const std::string &timestamp, const std::string &comments, int timeout, int zone, int id, int mark, int helper, int expect, int mask, int flags, int use, int use_expect, int use_helper, int use_zone)> callback)
        {
            policy_callbacks_.push_back(callback);
        }
        void connection::remove_policy_callback( std::function<void(const std::string &src_ip, const std::string &dst_ip, const std::string &src_port, const std::string &dst_port, const std::string &protocol, const std::string &state, const std::string &direction, const std::string &timestamp, const std::string &comments, int timeout, int zone, int id, int mark, int helper, int expect, int mask, int flags, int use, int use_expect, int use_helper, int use_zone)> callback)
        {
            //std::find callback in policy callbacks and remove it
            for ( auto it = policy_callbacks_.begin(); it != policy_callbacks_.end(); ++it)
            {
                if (&(*it) == &callback)
                {
                    policy_callbacks_.erase(it);
                    break;
                }
            }       
        }
        //destructor


        
     
    }//namespace conntrack

} // namespace happy_proxy
// Path: src/conntrack/conntrack_listener.cpp
