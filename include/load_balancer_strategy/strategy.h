#ifndef __STRATEGY_H_
#define __STRATEGY_H_

#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>
#include <fstream>
#include <chrono>
#include <random>
//route strategy is used to decide which server to use for a request 
//when a routing policy is in place, the strategy is not neccessary.
//the routing policy will be used to decide which server to use for a request 
#include "proxy.h"

namespace happy_proxy
{
  namespace load_balancer
  {

    //strategy pattern:
    struct strategy_details
    {
      std::string name;
      std::string description;
      std::string type;
      std::string version;
      std::string author;
      std::string license;
      std::string date;
      std::string time;
      std::string comments;
    };
    struct strategy_server_statistics
    {
      uint64_t server_id;
      int16_t server_port;
      std::string server_ip;
      int64_t server_connections;
      int64_t server_bandwidth;
      int64_t server_packet_loss;
      int64_t server_latency;
      int64_t server_jitter;
      int64_t server_error_rate;
      int64_t server_retransmission_rate;
      int64_t server_retransmission_time;
      int64_t server_retransmission_count;
      int64_t server_retransmission_window;
      int64_t server_retransmission_packet;
      int64_t server_retransmission_byte;


      std::chrono::time_point<std::chrono::system_clock> server_last_response_time; 
      std::chrono::time_point<std::chrono::system_clock> server_last_request_time;       
      //  
      
    };
    class strategy
    {
    protected:
      strategy_details details_;
      std::vector<happy_proxy::server_details> servers_;
      std::mutex mutex_;
      std::atomic<bool> updated_; 
      bool _sticky_session_;


    public:
      strategy(const strategy_details& details) : details_(details),_sticky_session_(false) {}
      strategy(const strategy_details& details, const std::vector<happy_proxy::server_details>& servers) : details_(details), servers_(servers) {} 
      strategy(const strategy& other); 
      strategy();
      strategy& operator=(const strategy& other);
      strategy(strategy&& other);
      strategy& operator=(strategy&& other);

      virtual ~strategy()  ;
      virtual void apply() = 0; 
      virtual strategy* clone() = 0;
      virtual void add_server(const struct happy_proxy::server_details& server);
      virtual void remove_server(const struct happy_proxy::server_details& server);
      virtual void update_server(const struct happy_proxy::server_details& server);

    };
    class performance_strategy : virtual public strategy
    {   
        protected: 
        std::map<int64_t,struct strategy_server_statistics> server_statistics_; 
        std::mutex server_statistics_mutex_; 
        std::atomic<bool> server_statistics_updated_; 
        std::thread server_statistics_updater_;
        size_t current_index_ = 0;
        int64_t current_server_ = 0;
        
        public:
        
        performance_strategy();
        
        performance_strategy(const strategy_details& details) : strategy(details) {} 
        
        performance_strategy(const strategy_details& details,const std::vector<happy_proxy::server_details>& servers ) : strategy(details,servers) {} 

        performance_strategy(const performance_strategy& other);  

        performance_strategy& operator=(const performance_strategy& other); 
        performance_strategy& operator=(performance_strategy&& other);

        performance_strategy(performance_strategy&& other);
        virtual ~performance_strategy();
        //update server statistics thread:
        void update_server_statistics(); 
        std::string get_next_server(); 
        //abstract functions: 
        virtual void apply() override;
        virtual strategy* clone() override;

          
    };
    class round_robin_strategy : virtual public strategy
    {

        size_t current_index_ = 0;
        int64_t current_server_ = 0;
       
        std::vector<server_details> servers_;

        
        
        public:
        std::string get_next_server();
        
        round_robin_strategy();
        round_robin_strategy(const round_robin_strategy& other);
        round_robin_strategy& operator=(const round_robin_strategy& other);
        round_robin_strategy(round_robin_strategy&& other);
        round_robin_strategy& operator=(round_robin_strategy&& other);
        round_robin_strategy(const strategy_details& details) : strategy(details) {} 
        virtual ~round_robin_strategy() ;
        //apply and clone:
        virtual void apply() override;
        virtual strategy* clone() override;
        protected:
        

    };
    class least_connection_strategy :virtual public strategy
    {
      protected:
      std::map<int64_t,struct strategy_server_statistics> server_statistics_; 
      std::mutex server_statistics_mutex_;
      std::atomic<bool> server_statistics_updated_;
      std::thread server_statistics_updater_;
      public:
      least_connection_strategy();
      least_connection_strategy(const least_connection_strategy& other);
      least_connection_strategy(const strategy_details& details) : strategy(details) {} 
      virtual ~least_connection_strategy();
      std::string get_next_server();
     
      //abstract functions:
      virtual void apply() override;
      virtual strategy* clone() override;

    };
    class random_strategy : virtual public strategy
    {
     std::random_device generator_;
     public:

      std::string get_next_server();
      
      random_strategy();
      random_strategy(const strategy_details& details) : strategy(details) {} 
      virtual ~random_strategy(){}
      //abstract functions:
      virtual void apply() override;
      virtual strategy* clone() override;


    };
    class weighted_strategy : virtual public strategy
    {

        public:
        
    };
    class ip_hash_strategy : virtual public strategy
    {
         public: 


    };
    class url_hash_strategy :virtual  public strategy
    {
      //url hash strategy
      //hash the url and use the hash to decide which server to use for a request

    };
    class least_response_time_strategy : virtual public strategy
    {
      //least response time strategy
      //use the server with the least response time for a request

    };
    class least_bandwidth_strategy :virtual public strategy
    {
      //least bandwidth strategy
      //use the server with the least bandwidth for a request

    };
    class least_packet_loss_strategy : virtual public strategy
    {
      //least packet loss strategy
      //use the server with the least packet loss for a request

    };
    class least_latency_strategy :virtual public strategy
    {
      //least latency strategy
      //use the server with the least latency for a request

    };
    class least_jitter_strategy :virtual public strategy
    {
      //least jitter strategy
      //use the server with the least jitter for a request

    };
    class least_error_rate_strategy : virtual public strategy
    {
      //least error rate strategy
      //use the server with the least error rate for a request
    };
    class least_retransmission_rate_strategy :virtual public strategy
    {
      //least retransmission rate strategy
      //use the server with the least retransmission rate for a request
    };
    class least_retransmission_time_strategy :virtual public strategy
    {
      //least retransmission time strategy
    };
    class least_retransmission_count_strategy :virtual public strategy
    {
      //least retransmission count strategy
    };
    class least_retransmission_window_strategy :virtual public strategy
    {
      //least retransmission window strategy

    };
    class least_retransmission_packet_strategy :virtual public strategy
    {
      //least retransmission packet strategy
    };
    class least_retransmission_byte_strategy :virtual public strategy
    {
      //least retransmission byte strategy
    };
    class sticky_session_strategy : virtual public strategy
    {
      protected:
      std::map<std::string, std::string> sticky_sessions_;
      std::mutex sticky_sessions_mutex_;
      std::atomic<bool> sticky_sessions_updated_;
      std::thread sticky_sessions_updater_;
     public:
      sticky_session_strategy();
      sticky_session_strategy(const strategy_details& details) : strategy(details) {} 
      virtual ~sticky_session_strategy(); 
      //add sticky session
      void add_sticky_session(const std::string& session_id, const std::string& server_id); 
      //remove sticky session
      void remove_sticky_session(const std::string& session_id);
      //update sticky session
      void update_sticky_session(const std::string& session_id, const std::string& server_id);
      //get next server
      std::string get_next_server(const std::string& session_id);
      //add server
      void add_server(const struct happy_proxy::server_details& server);
      //remove server
      void remove_server(const struct happy_proxy::server_details& server);
      //update server
      void update_server(const struct happy_proxy::server_details& server);
    };
    class strategy_ensemble {
      
      std::vector<strategy*> strategies_;
      std::mutex mutex_;
      std::atomic<bool> updated_;
      //weights for each strategy 
      std::map<strategy*, float> weights_; //map the strategies_ with their weights 

    public:
      strategy_ensemble()  ;
      strategy_ensemble(const std::vector<strategy>& strategies) ;
      strategy_ensemble(const strategy_ensemble& other);
      strategy_ensemble& operator=(const strategy_ensemble& other);
      strategy_ensemble(strategy_ensemble&& other);
      strategy_ensemble& operator=(strategy_ensemble&& other);
      virtual ~strategy_ensemble() ;
      //clone:
      strategy_ensemble* clone();
      void apply();
      void add_strategy(const strategy& strategy, float weight);
      void remove_strategy(const strategy& strategy);
      void update_strategy(const strategy& strategy, float weight);
    };
  }//load_balancer

}//happy_proxy
#endif