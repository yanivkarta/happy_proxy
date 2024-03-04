#include "conntrack/conntrack_policy_maker.h"

namespace happy_proxy
{ 
    namespace conntrack
    {
        //default listener constructor:
        conntrack_listener::conntrack_listener():handle_(nullptr), qh_(nullptr), ct_(nullptr), tuple_(nullptr), tcp_info_(nullptr), exp_(nullptr), helper_(nullptr), zone_(nullptr), conntrack_thread_(nullptr), conntrack_running_(false)
        {
            //initialize the listener
            initialize();

        }
        
        //initialize the listener:
        void conntrack_listener::initialize()
        {
            //initialize the netfilter connection tracking handle:
            this->handle_ = nfct_open(CONNTRACK, 0);
            if (!handle_)
            {
                throw std::runtime_error("Failed to open netfilter connection tracking handle");
            }
            //initialize the netfilter connection tracking queue handle:
            this->qh_ = nfct_q_create_handle(handle_);
            if (!qh_)
            {
                throw std::runtime_error("Failed to create netfilter connection tracking queue handle");
            }
            //initialize the netfilter connection tracking tuple:
            this->tuple_ = nfct_tuple_alloc();
            if (!tuple_)
            {
                throw std::runtime_error("Failed to allocate netfilter connection tracking tuple");
            }
            //initialize the netfilter connection tracking tcp info:
            this->tcp_info_ = nfct_tcp_alloc();
            if (!tcp_info_)
            {
                throw std::runtime_error("Failed to allocate netfilter connection tracking tcp info");
            }
            //initialize the netfilter connection tracking expect:
            this->exp_ = nfct_expect_alloc();
            if (!exp_)
            {
                throw std::runtime_error("Failed to allocate netfilter connection tracking expect");
            }
            //initialize the netfilter connection tracking helper:
            this->helper_ = nfct_helper_alloc();
            if (!helper_)
            {
                throw std::runtime_error("Failed to allocate netfilter connection tracking helper");
            }
            //initialize the netfilter connection tracking zone:
            this->zone_ = nfct_zone_alloc();
            if (!zone_)
            {
                throw std::runtime_error("Failed to allocate netfilter connection tracking zone");
            }
            //initialize the log file:
            
            conntrack_log_.open(log_file_, std::ios::out | std::ios::app);
            if (!conntrack_log_.is_open())
            {
                throw std::runtime_error("Failed to open conntrack log file");
            }
            //initialize the conntrack thread:
            conntrack_thread_ = new std::thread(&conntrack_listener::conntrack_thread, this);
            if (!conntrack_thread_)
            {
                throw std::runtime_error("Failed to create conntrack thread");
            }
            //initialize the conntrack running flag:
            conntrack_running_ = true;
        }   
        //run the conntrack thread:
        void conntrack_listener::conntrack_thread()
        {
            //run the conntrack thread:
            while (conntrack_running_)
            {
                //run the conntrack listener:
                run();
            }
        }       
        //run the conntrack listener:
        void conntrack_listener::run()
        {
            //run the conntrack listener:
            int ret = nfct_callback_register(qh_, NFCT_T_ALL, &conntrack_listener::conntrack_callback, this);
            if (ret < 0)
            {
                throw std::runtime_error("Failed to register conntrack callback");
            }
            //run the conntrack listener queue:
            ret = nfct_q_handle_input(qh_);
            if (ret < 0)
            {
                throw std::runtime_error("Failed to handle conntrack queue input");
            }
            else
            {
                //log the conntrack listener queue input:
                conntrack_log_ << "Handled conntrack queue input" << std::endl;
            }   

        }   
        //conntrack input/output  handler: 
        
    } // namespace conntrack    

} // namespace happy_proxy
