#include "conntrack/conntrack_policy_maker.h"
#include "conntrack/conntrack_listener.h"

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <thread>
#include <atomic>
#include <fstream>
#include <mutex>
#include <map>
#include <libnfnetlink/libnfnetlink.h>
//#include <libnetfilter_conntrack/libnetfilter_conntrack.h>
//#include <libnetfilter_conntrack/libnetfilter_conntrack_tcp.h>
//for nfct_q_create_handle:
//#include <libnetfilter_conntrack/libnetfilter_conntrack_q.h>


#include <linux/netfilter/nfnetlink_queue.h>
#include <linux/netfilter/nfnetlink.h>
#include <linux/netfilter/nf_conntrack_common.h>
#include <linux/netfilter/nf_conntrack_tuple_common.h>

//for nfct_callback_register:
#include <libnetfilter_conntrack/libnetfilter_conntrack.h>
#include <linux/netfilter/nf_conntrack_common.h>
#include <linux/netfilter/nf_conntrack_tuple_common.h>
#include <linux/netfilter/nf_conntrack_tcp.h>
#include <linux/netfilter/xt_conntrack.h>


//for nf_conntrack_type:
#include <linux/netfilter/nf_conntrack_common.h>
namespace happy_proxy
{ 
    namespace conntrack
    {
        
        conntrack_listener::conntrack_listener(): handle_(nullptr), qh_(nullptr), ct_(nullptr), tuple_(nullptr), tcp_info_(nullptr), exp_(nullptr), helper_(nullptr), zone_(nullptr), conntrack_thread_(nullptr), n_conntrack_threads_(1), conntrack_running_(false), conntrack_mutex_(), backends_(), frontends_(), log_file_("/var/log/happy_proxy/conntrack.log"), conntrack_log_(), log_mutex_(), callback_mutex_(), policies_()
        {
            //initialize the conntrack listener:
            initialize();
        }   
        //listener constructor:
        //initialize the listener:
        void conntrack_listener::initialize()
        {
            //initialize the netfilter connection tracking handle:
            this->handle_ = nfct_open(CONNTRACK, 0);
            if (!handle_)
            {
                throw std::runtime_error("Failed to open netfilter connection tracking handle");
            }
             

            conntrack_log_.open(log_file_, std::ios::out | std::ios::app);
            if (!conntrack_log_.is_open())
            {
                throw std::runtime_error("Failed to open conntrack log file");
            }
            //initialize the conntrack thread:
            conntrack_thread_ = new std::thread(&conntrack_listener::conntrack_thread, this);
            if (!conntrack_thread_)
            {
                //conntrack_thread reads from the queue and processes the conntrack events 
                //and push the event to the policy maker
                //if the conntrack thread fails to start, throw an exception:
                throw std::runtime_error("Failed to create conntrack thread");
            }
            //initialize the conntrack running flag:
            conntrack_running_ = true;

            //initialize the queue handler:
            if( handle_ != nullptr) {

                qh_ = nfct_q_create_handle((struct nfct_handle*)handle_);
                if (!qh_)
                {

                    throw std::runtime_error("Failed to create conntrack queue handle");
                }   
            
            }   


        }   
         void conntrack_listener::conntrack_thread()
        {
            //run the conntrack thread:
            //lock the mutex:
            std::lock_guard<std::mutex> lock(conntrack_mutex_);
            //run the conntrack listener:
            
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
            //lock the mutex:
            std::lock_guard<std::mutex> lock(conntrack_mutex_);
            //run the conntrack listener:
            //run the conntrack queue handler:
            if( qh_ != nullptr) {
                nfct_q_handle_input((nfct_q_handle*)qh_);
            }
        }
        //finalize the conntrack listener:
        void conntrack_listener::finalize()
        {
            //finalize the conntrack listener:
            //lock the mutex:
            std::lock_guard<std::mutex> lock(conntrack_mutex_);
            //finalize the conntrack listener:
            //finalize the conntrack queue handler:
            if( qh_ != nullptr) {
                nfct_q_handle_output((nfct_q_handle*)qh_);
            }
            //finalize the conntrack listener:
            //finalize the conntrack thread:
            if (conntrack_thread_)
            {
                conntrack_thread_->join();
                delete conntrack_thread_;
                conntrack_thread_ = nullptr;
            }
            //finalize the conntrack listener:
            //finalize the netfilter connection tracking handle:
            if (handle_)
            {
                nfct_close((nfct_handle*)handle_);
                handle_ = nullptr;  
            }
            //finalize the conntrack listener:
            //finalize the conntrack queue handler:
            if( qh_ != nullptr) {
                nfct_q_handle_output((nfct_q_handle*)qh_);  
            }
        }//conntrack callback:
        int conntrack_listener::conntrack_callback(const struct nlmsghdr *nlh, void *data)
        {
            std::cout << "[+]conntrack callback:"<<std::to_string(nlh->nlmsg_type)<< "," <<std::to_string(nlh->nlmsg_len)<<","<<std::to_string(nlh->nlmsg_flags)<<","<<std::to_string(nlh->nlmsg_seq)<<","<<std::to_string(nlh->nlmsg_pid)<<std::endl; 
            //convert data pointer:
            //conntrack_listener* listener = static_cast<conntrack_listener*>(data); 
            //conntrack callback:
            std::cout<<"[+]conntrack callback data pointer:"<<std::hex<<std::to_string((uint64_t)data)<<std::dec<<std::endl; 

            //
            return 0;
        }   
        //conntrack handler:
        void conntrack_listener::conntrack_handler()
        {
            //conntrack handler:
            //lock the mutex:
            
            //log the conntrack listener queue input:
            std::cout << "Handled conntrack queue input" << std::endl;
            
        }   
        //add a backend:
        void conntrack_listener::add_backend(const std::string& ip, const std::string& port)
        {
            //add a backend:
            //lock the mutex:
            std::lock_guard<std::mutex> lock(callback_mutex_);
            //add a backend:
            //add the backend to the backends map:
            backends_[ip] = port;
        }   
        //remove a backend:
        void conntrack_listener::remove_backend(const std::string& ip, const std::string& port)
        {
            //remove a backend:
            //lock the mutex:
            std::lock_guard<std::mutex> lock(callback_mutex_);
            //remove a backend:
            //remove the backend from the backends map:
            //get instances of ip-port: 
            auto key = ip+":"+port;
            if(backends_.find(key) != backends_.end()) {
                backends_.erase(key);
            }  

            

        }   
        //add a frontend:
        void conntrack_listener::add_frontend(const std::string& ip, const std::string& port)
        {
            //add a frontend:
            //lock the mutex:
            std::lock_guard<std::mutex> lock(callback_mutex_);
            //add a frontend:
            //add the frontend to the frontends map:
            frontends_[ip+":"+port] = port;
        }
        //remove a frontend:
        void conntrack_listener::remove_frontend(const std::string& ip, const std::string& port)
        {
            //remove a frontend:
            //lock the mutex:
            std::lock_guard<std::mutex> lock(callback_mutex_);
            //remove a frontend:
            //remove the frontend from the frontends map:
            frontends_.erase(ip+":"+port);
        }   
        //add a policy:
        void conntrack_listener::add_policy(const std::string& src_ip, const std::string& dst_ip, const std::string& src_port, const std::string& dst_port, const std::string& protocol, const std::string& state, const std::string& direction, const std::string& timestamp, const std::string& comments, int timeout, int zone, int id, int mark, int helper, int expect, int mask, int flags, int use, int use_expect, int use_helper, int use_zone)
        {
            //add a policy:
            //lock the mutex:
            std::lock_guard<std::mutex> lock(callback_mutex_);
            //add a policy:
            //create a new connection:
            connection_details details;
            details.src_ip = src_ip;
            details.dst_ip = dst_ip;
            details.src_port = src_port;
            details.dst_port = dst_port;
            details.protocol = protocol;
            details.state = state;
            details.direction = direction;
            details.timestamp = timestamp;
            details.comments = comments;
            details.timeout = timeout;
            details.zone = zone;
            details.id = id;
            details.mark = mark;
            details.helper = helper;
            details.expect = expect;
            details.mask = mask;
            details.flags = flags;
            details.use = use;
            details.use_expect = use_expect;
            details.use_helper = use_helper;
            details.use_zone = use_zone;
            connection* conn = new connection(details);
            //add the policy to the policies map:
            policies_[src_ip + dst_ip + src_port + dst_port + protocol + state + direction + timestamp + comments] = conn;
        }   
        //remove a policy:
        void conntrack_listener::remove_policy(const std::string& src_ip, const std::string& dst_ip, const std::string& src_port, const std::string& dst_port, const std::string& protocol, const std::string& state, const std::string& direction, const std::string& timestamp, const std::string& comments, int timeout, int zone, int id, int mark, int helper, int expect, int mask, int flags, int use, int use_expect, int use_helper, int use_zone)
        {
            //remove a policy:
            //lock the mutex:
            std::lock_guard<std::mutex> lock(callback_mutex_);


            //remove a policy:
            //remove the policy from the policies map:
            //print the input to avoid unused  warnings:
            std::cout << "Removing policy : " << src_ip << dst_ip << src_port << dst_port << protocol << state << direction << timestamp << comments << timeout << zone << id << mark << helper << expect << mask << flags << use << use_expect << use_helper << use_zone;

            auto key = src_ip + dst_ip + src_port + dst_port + protocol + state + direction + timestamp + comments; 
            if(policies_.find(key) != policies_.end()) {
                connection* conn = policies_[key]; 
                conn->remove_policy_callback(remove_policy_callback_);
                //remove the policy from the policies map:
                policies_.erase(key);
                //delete the connection, don't use after free ;-)
                
                delete conn;

            }
            
            std::cout << "Removed policy : " << key;

            
        }   
        //add a backend callback:   
        void conntrack_listener::add_backend_callback( std::function<void(const std::string &ip, const std::string &port)> callback)
        {
            //add a backend callback:
            //lock the mutex:
            std::lock_guard<std::mutex> lock(callback_mutex_);
            //add a backend callback:
            add_backend_callback_ = callback;
        }   
        //remove a backend callback:
        void conntrack_listener::remove_backend_callback( std::function<void(const std::string &ip, const std::string &port)> callback)
        {
            //remove a backend callback:
            //lock the mutex:
            std::lock_guard<std::mutex> lock(callback_mutex_);
            //remove a backend callback:
            remove_backend_callback_ = callback;
        }   
        //add a frontend callback:
        void conntrack_listener::add_frontend_callback( std::function<void(const std::string &ip, const std::string &port)> callback)
        {
            //add a frontend callback:
            //lock the mutex:
            std::lock_guard<std::mutex> lock(callback_mutex_);
            //add a frontend callback:
            add_frontend_callback_ = callback;
        }   
        //remove a frontend callback:   
        void conntrack_listener::remove_frontend_callback( std::function<void(const std::string &ip, const std::string &port)> callback)
        {
            //remove a frontend callback:
            //lock the mutex:
            std::lock_guard<std::mutex> lock(callback_mutex_);
            //remove a frontend callback:
            remove_frontend_callback_ = callback;
        }   
        //add a policy callback:    
        void conntrack_listener::add_policy_callback( std::function<void(const std::string &src_ip, const std::string &dst_ip, const std::string &src_port, const std::string &dst_port, const std::string &protocol, const std::string &state, const std::string &direction, const std::string &timestamp, const std::string &comments, int timeout, int zone, int id, int mark, int helper, int expect, int mask, int flags, int use, int use_expect, int use_helper, int use_zone)> callback)
        {
            //add a policy callback:
            //lock the mutex:
            std::lock_guard<std::mutex> lock(callback_mutex_);
            //add a policy callback:
            add_policy_callback_ = callback;
        }   
        //remove a policy callback: 
        void conntrack_listener::remove_policy_callback( std::function<void(const std::string &src_ip, const std::string &dst_ip, const std::string &src_port, const std::string &dst_port, const std::string &protocol, const std::string &state, const std::string &direction, const std::string &timestamp, const std::string &comments, int timeout, int zone, int id, int mark, int helper, int expect, int mask, int flags, int use, int use_expect, int use_helper, int use_zone)> callback)
        {
            //remove a policy callback:
            //lock the mutex:
            std::lock_guard<std::mutex> lock(callback_mutex_);
            //remove a policy callback:
            remove_policy_callback_ = callback;
        }   
        //copy constructor:
        conntrack_listener::conntrack_listener(const conntrack_listener& other): handle_(other.handle_), qh_(other.qh_), ct_(other.ct_), tuple_(other.tuple_), tcp_info_(other.tcp_info_), exp_(other.exp_), helper_(other.helper_), zone_(other.zone_), add_backend_callback_(other.add_backend_callback_), remove_backend_callback_(other.remove_backend_callback_), add_frontend_callback_(other.add_frontend_callback_), remove_frontend_callback_(other.remove_frontend_callback_), add_policy_callback_(other.add_policy_callback_), remove_policy_callback_(other.remove_policy_callback_), conntrack_thread_(other.conntrack_thread_), n_conntrack_threads_(other.n_conntrack_threads_), conntrack_running_((bool)other.conntrack_running_), conntrack_mutex_(), backends_(other.backends_), frontends_(other.frontends_), log_file_(other.log_file_), conntrack_log_(), log_mutex_(), callback_mutex_(), policies_(other.policies_)
        {
            //copy constructor:
            //initialize the conntrack listener:
            initialize();
        }   
        //copy assignment operator:
        conntrack_listener& conntrack_listener::operator=(const conntrack_listener& other)
        {
            //copy assignment operator:
            if (this != &other)
            {
                //finalize the conntrack listener:
                finalize();
                //copy the handle:
                handle_ = other.handle_;
                qh_ = other.qh_;
                ct_ = other.ct_;
                tuple_ = other.tuple_;
                tcp_info_ = other.tcp_info_;
                exp_ = other.exp_;
                helper_ = other.helper_;
                zone_ = other.zone_;
                add_backend_callback_ = other.add_backend_callback_;
                remove_backend_callback_ = other.remove_backend_callback_;
                add_frontend_callback_ = other.add_frontend_callback_;
                remove_frontend_callback_ = other.remove_frontend_callback_;
                add_policy_callback_ = other.add_policy_callback_;
                remove_policy_callback_ = other.remove_policy_callback_;
                conntrack_thread_ = other.conntrack_thread_;
                n_conntrack_threads_ = other.n_conntrack_threads_;
                conntrack_running_.exchange((bool)other.conntrack_running_);    
                backends_ = other.backends_;
                frontends_ = other.frontends_;
                log_file_ = other.log_file_;
                //conntrack_log_ = std::move(other.conntrack_log_);
                policies_ = other.policies_;
                //initialize the conntrack listener:
                initialize();
            }
            return *this;
        }   
        //move constructor: 
        conntrack_listener::conntrack_listener(conntrack_listener&& other): handle_(other.handle_), qh_(other.qh_), ct_(other.ct_), tuple_(other.tuple_), tcp_info_(other.tcp_info_), exp_(other.exp_), helper_(other.helper_), zone_(other.zone_), add_backend_callback_(other.add_backend_callback_), remove_backend_callback_(other.remove_backend_callback_), add_frontend_callback_(other.add_frontend_callback_), remove_frontend_callback_(other.remove_frontend_callback_), add_policy_callback_(other.add_policy_callback_), remove_policy_callback_(other.remove_policy_callback_), conntrack_thread_(other.conntrack_thread_), n_conntrack_threads_(other.n_conntrack_threads_), conntrack_running_((bool)other.conntrack_running_), conntrack_mutex_(), backends_(other.backends_), frontends_(other.frontends_), log_file_(other.log_file_), conntrack_log_(), log_mutex_(), callback_mutex_(), policies_(other.policies_)
        {
            //move constructor:
            //initialize the conntrack listener:
            initialize();
            //finalize the conntrack listener:
            other.finalize();
        }   
        //move assignment operator:
        conntrack_listener& conntrack_listener::operator=(conntrack_listener&& other)
        {
            //move assignment operator:
            if (this != &other)
            {
                //finalize the conntrack listener:
                finalize();
                //move the handle:
                handle_ = other.handle_;
                qh_ = other.qh_;
                ct_ = other.ct_;
                tuple_ = other.tuple_;
                tcp_info_ = other.tcp_info_;
                exp_ = other.exp_;
                helper_ = other.helper_;
                zone_ = other.zone_;
                add_backend_callback_ = other.add_backend_callback_;
                remove_backend_callback_ = other.remove_backend_callback_;
                add_frontend_callback_ = other.add_frontend_callback_;
                remove_frontend_callback_ = other.remove_frontend_callback_;
                add_policy_callback_ = other.add_policy_callback_;
                remove_policy_callback_ = other.remove_policy_callback_;
                conntrack_thread_ = other.conntrack_thread_;
                n_conntrack_threads_ = other.n_conntrack_threads_;
                conntrack_running_.exchange((bool)other.conntrack_running_);
                backends_ = other.backends_;
                frontends_ = other.frontends_;
                log_file_ = other.log_file_;
                //conntrack_log_ = other.conntrack_log_;
                policies_ = other.policies_;
                //initialize the conntrack listener:
                initialize();
                //finalize the conntrack listener:
                other.finalize();
            }
            return *this;
        }   
        //destructor:
        conntrack_listener::~conntrack_listener()
        {
            //destructor:
            //finalize the conntrack listener:
            finalize();
        }   
        //static helpers :
        //nfct_q_handle_input:
        int conntrack_listener::nfct_q_handle_input(nfct_q_handle *qh)
        {
            //nfct_q_handle_input:
            //run the nfct queue handle input:
            return nfct_q_handle_input(qh);
        }   
        //nfct_q_handle_output:
        int conntrack_listener::nfct_q_handle_output(nfct_q_handle *qh)
        {
            //nfct_q_handle_output:
            //run the nfct queue handle output:
            return nfct_q_handle_output(qh);
        }   
        //nfct_q_create_handle:
        nfct_q_handle* conntrack_listener::nfct_q_create_handle(nfct_handle *h)
        {
            //nfct_q_create_handle:
            //create a new nfct queue handle:
            return nfct_q_create_handle(h);
        }   
      } // namespace conntrack    

} // namespace happy_proxy
