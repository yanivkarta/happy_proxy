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
//#include <libnetfilter_conntrack/libnetfilter_conntrack.h>
//#include <libnetfilter_conntrack/libnetfilter_conntrack_tcp.h>
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
#include <ctime>
#include <map>
#include <mutex>
#include <thread>


#include "conntrack_policy_maker.h"


#include <libnfnetlink/libnfnetlink.h>

#include <linux/netfilter/nf_conntrack_common.h>
#include <linux/netfilter/nf_conntrack_tuple_common.h>
//conntrackfilter
#include <linux/netfilter/nf_conntrack_tcp.h>

#include <linux/netfilter/nfnetlink.h>
#include <linux/netfilter/nfnetlink_queue.h>

//zone,tuple,helper,expect:
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

namespace happy_proxy
{
    namespace conntrack
    {

        
        class conntrack_listener
        {
        private:
            //conntrack handle


            void *handle_;
            conntrack::nfct_q_handle *qh_;
            void *ct_;
            void *tuple_;
            void *tcp_info_;
            void *exp_;
            void *helper_;
            void *zone_;
            //callbacks
            std::function<void(const std::string &ip, const std::string &port)> add_backend_callback_; 
            std::function<void(const std::string &ip, const std::string &port)> remove_backend_callback_; 
            std::function<void(const std::string &ip, const std::string &port)> add_frontend_callback_; 
            std::function<void(const std::string &ip, const std::string &port)> remove_frontend_callback_; 
            std::function<void(const std::string &src_ip, const std::string &dst_ip, const std::string &src_port, const std::string &dst_port, const std::string &protocol, const std::string &state, const std::string &direction, const std::string &timestamp, const std::string &comments, int timeout, int zone, int id, int mark, int helper, int expect, int mask, int flags, int use, int use_expect, int use_helper, int use_zone)> add_policy_callback_; 
            std::function<void(const std::string &src_ip, const std::string &dst_ip, const std::string &src_port, const std::string &dst_port, const std::string &protocol, const std::string &state, const std::string &direction, const std::string &timestamp, const std::string &comments, int timeout, int zone, int id, int mark, int helper, int expect, int mask, int flags, int use, int use_expect, int use_helper, int use_zone)> remove_policy_callback_; 


        //log
        //thread
        std::thread *conntrack_thread_; //array of threads
        size_t n_conntrack_threads_;//number of threads


        std::atomic<bool> conntrack_running_  = false;
        std::mutex conntrack_mutex_;
        std::map<std::string, std::string> backends_;
        std::map<std::string, std::string> frontends_;
        std::string log_file_;
        std::ofstream conntrack_log_;
        std::mutex log_mutex_;
        //callback mutex
        std::mutex callback_mutex_;
        //policies
        std::map<std::string, connection*> policies_;
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
            //Q
            
            static int Q(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfa, void *data);
            //run   
            void run();
            virtual void finalize();
            void conntrack_thread(); 
             void conntrack_handler();
            //conntrack callback static:
            static int conntrack_callback(const struct nlmsghdr *nlh, void *data); 
            //nfct_q_handle helpers :

            //create handle : 
             static nfct_q_handle* nfct_q_create_handle(nfct_handle* h);
            //nfct_q_handle_input:
            static int nfct_q_handle_input(nfct_q_handle *qh);
            //nfct_q_handle_output:
            static int nfct_q_handle_output(nfct_q_handle *qh);
            //nfct_q_handle_error:
            static int nfct_q_handle_error(nfct_q_handle *qh);
            //nfct_q_handle_timeout:
            static int nfct_q_handle_timeout(nfct_q_handle *qh);
            //nfct_q_handle_signal:
            static int nfct_q_handle_signal(nfct_q_handle *qh);
            //nfct_q_handle_event:
            static int nfct_q_handle_event(nfct_q_handle *qh);
            //nfct_q_handle_event_base:
            static int nfct_q_handle_event_base(nfct_q_handle *qh);
            //nfct_q_handle_event_loop:
            static int nfct_q_handle_event_loop(nfct_q_handle *qh);
            //nfct_q_handle_event_base_loop:
            static int nfct_q_handle_event_base_loop(nfct_q_handle *qh);
            //nfct_q_handle_event_base_loopexit:
            static int nfct_q_handle_event_base_loopexit(nfct_q_handle *qh);
            //nfct_q_handle_event_base_loopbreak:
            static int nfct_q_handle_event_base_loopbreak(nfct_q_handle *qh);
            //nfct_q_handle_event_base_loopcontinue:
            static int nfct_q_handle_event_base_loopcontinue(nfct_q_handle *qh);
            
        };
    }//conntrack
} //happy_proxy 

#endif
