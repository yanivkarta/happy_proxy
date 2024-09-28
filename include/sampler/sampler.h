#ifndef __SAMPER_H_
#define __SAMPER_H_

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
#include <functional>
#include <iomanip>
#include <sstream>
//for server_details:
#include "proxy.h"
#include "proxy_util.h"

//sampler is a class that samples the snmp statistics from the monitored resources ( servers,backends,frontends,etc)   
//and analyzes the statistics to determine the health of the resources. 
//The sampler is a part of the load balancer strategy and only when performance based prediction is used it is activated. 

namespace happy_proxy
{
  namespace load_balancer
  {

    enum sampler_status
    {
      SAMPLER_OK = 0,
      SAMPLER_WARNING = 1,
      SAMPLER_CRITICAL = 2,
      SAMPLER_UNKNOWN = 3
    };
    enum sampler_type
    {
      SAMPLER_TYPE_CPU = 0,
      SAMPLER_TYPE_MEMORY = 1,
      SAMPLER_TYPE_DISK = 2,
      SAMPLER_TYPE_NETWORK = 3,
      SAMPLER_TYPE_OTHER = 4
    };

    //sampling method:
    enum sampler_method
    {
      SAMPLER_METHOD_SNMP = 0,
      SAMPLER_METHOD_RTSTATS = 1,
      SAMPLER_METHOD_LOCAL = 2,
      SAMPLER_METHOD_OTHER = 3
    };

    //sampler statistics 
    //map of statistics name and value on timepoint 

    template <typename T>
    class sample 
    {
      std::map<std::string, T> statistics_;
      std::chrono::time_point<std::chrono::system_clock> timestamp_;
      public:
      //constructor
      sample();
      //copy constructor
      sample(const sample& other);
      //copy assignment operator
      sample& operator=(const sample& other);
      //move constructor
      sample(sample&& other);
      //destructor
      ~sample();
      //get the statistics
      std::map<std::string, T> get_statistics()const;
      //set the statistics
      void set_statistics(std::map<std::string, T> statistics);
      //get the timestamp
      std::chrono::time_point<std::chrono::system_clock> get_timestamp()const;
      //set the timestamp
      void set_timestamp(std::chrono::time_point<std::chrono::system_clock> timestamp);
    };  

    

    
    class sampler
    {
    protected:

      //context:
      std::chrono::time_point<std::chrono::system_clock> start_time_; 
      std::chrono::time_point<std::chrono::system_clock> end_time_; 
      int64_t time_elapsed_;

      //statistics:
      std::chrono::time_point<std::chrono::system_clock> created_; 
      std::map<int64_t, std::map<std::string, std::string>> statistics_; //timestamp, statistics 
      std::chrono::time_point<std::chrono::system_clock> last_sampled_; 
      std::chrono::time_point<std::chrono::system_clock> last_analyzed_;
      
      std::mutex monitor_mutex_;
      std::function<size_t (sampler&)> sample_callback_;
      std::function<size_t (sampler&)> analyze_callback_;
      //sampling interval:
      int64_t sample_interval_;
    public:
      //constructor
      sampler();
      //copy constructor
      sampler(const sampler& other);
      //copy assignment operator
      sampler& operator=(const sampler& other);
      //move constructor
      sampler(sampler&& other);
      //destructor
      ~sampler();

      //sample the statistics from the monitored resources
      void sample_statistics();
      //analyze the statistics to determine the health of the resources
      void analyze_statistics();
      //get the statistics from the sampler
      std::map<int64_t, std::map<std::string, std::string>> get_statistics()const;
      //set the statistics for the sampler
      void set_statistics(std::map<int64_t, std::map<std::string, std::string>> statistics);
      //get the sampler statistics
      std::chrono::time_point<std::chrono::system_clock> get_last_sampled()const; 
      //set the sampler statistics
      void set_sampler_statistics(std::map<int64_t, std::map<std::string, std::string>> sampler_statistics);
      //get the sampler statistics
      const std::map<int64_t, std::map<std::string, std::string>>& get_sampler_statistics()const;

      //set the callbacks:
      //set the sample callback
      void set_sample_callback(std::function<size_t (sampler&)> sample_callback);
      //get the sample callback
      std::function<size_t (sampler&)> get_sample_callback()const;
      //set the analyze callback
      void set_analyze_callback(std::function<size_t (sampler&)> analyze_callback);
      //get the analyze callback
      std::function<size_t (sampler&)> get_analyze_callback()const;

      //set the sample interval
      void set_sample_interval(int64_t sample_interval);
      //get the sample interval
      int64_t get_sample_interval()const;
      
    };

    //specialized timers : 
    class sampler_timer :   public sampler
    {
        std::chrono::time_point<std::chrono::system_clock> start_time_; 
        std::chrono::time_point<std::chrono::system_clock> end_time_; 
        int64_t time_elapsed_;
 
        std::function<size_t (sampler_timer&)> start_callback_; 
        std::function<size_t (sampler_timer&)> stop_callback_;
        std::function<size_t (sampler_timer&)> reset_callback_;
        std::function<size_t (sampler_timer&)> update_callback_;
    public:
      //constructor
      sampler_timer();
      //copy constructor
      sampler_timer(const sampler_timer& other);
      //copy assignment operator
      sampler_timer& operator=(const sampler_timer& other);
      //move constructor
      sampler_timer(sampler_timer&& other);
      //destructor
      ~sampler_timer();
      //start the timer
      void start();
      //stop the timer
      void stop();
      //get the time elapsed
      int64_t get_time_elapsed();
      //set the time elapsed
      void set_time_elapsed(int64_t time_elapsed);
      //reset the timer
      void reset();
      //update the timer
      void update();
      //get the start time
      std::chrono::time_point<std::chrono::system_clock> get_start_time()const;
      //set the start time
      void set_start_time(std::chrono::time_point<std::chrono::system_clock> start_time);
      //get the end time
      std::chrono::time_point<std::chrono::system_clock> get_end_time()const;
      //set the end time
      void set_end_time(std::chrono::time_point<std::chrono::system_clock> end_time);
      //get the start callback
      std::function<size_t (sampler_timer&)> get_start_callback()const;
      //set the start callback
      void set_start_callback(std::function<size_t (sampler_timer&)> start_callback);
      //get the stop callback
      std::function<size_t (sampler_timer&)> get_stop_callback()const;
      //set the stop callback
      void set_stop_callback(std::function<size_t (sampler_timer&)> stop_callback);
         
    };
    class snmp_sampler : public sampler_timer
    {
      std::vector<happy_proxy::server_details> servers_;
      //lock when server_ change or when new statistics are added 

      struct snmp_session *session_;
      
      std::mutex monitor_mutex_;

      public:
      //constructor
      snmp_sampler();
      //copy constructor
      snmp_sampler(const snmp_sampler& other);
      //copy assignment operator
      snmp_sampler& operator=(const snmp_sampler& other);
      //move constructor
      snmp_sampler(snmp_sampler&& other);
      //destructor
      virtual ~snmp_sampler();
      //add server to the snmp sampler
      void add_server(const server_details& server);
      //remove server from the snmp sampler
      void remove_server(const server_details& server);
      //update server in the snmp sampler
      void update_server(const server_details& server);
      //get the servers from the snmp sampler
      std::vector<server_details> get_servers()const;
      //set the servers for the snmp sampler
      void set_servers(std::vector<server_details> servers);
      //get the servers from the snmp sampler
    };
    //specialized snmp sampler for cpu statistics 

    class rtstats_sampler : public sampler_timer
    {
      std::vector<happy_proxy::server_details> servers_;
      //lock when server_ change or when new statistics are added 
      std::mutex monitor_mutex_;
      public:
      //constructor
      rtstats_sampler();
      //copy constructor
      rtstats_sampler(const rtstats_sampler& other);
      //copy assignment operator
      rtstats_sampler& operator=(const rtstats_sampler& other);
      //move constructor
      rtstats_sampler(rtstats_sampler&& other);
      //destructor
      virtual ~rtstats_sampler();
      //add server to the rtstats sampler
      void add_server(const server_details& server);
      //remove server from the rtstats sampler
      void remove_server(const server_details& server);
      //update server in the rtstats sampler
      void update_server(const server_details& server);
      //get the servers from the rtstats sampler
      std::vector<server_details> get_servers()const;
      //set the servers for the rtstats sampler
      void set_servers(std::vector<server_details> servers);
      //get the servers from the rtstats sampler
    };

    //local statistics samplers: 
    //servers are added as context to the sampler 
    //statitics can automatically filter the servers based on the context 
    //and sample the statistics from the servers. 


    class local_sampler : public sampler_timer
    {
      std::vector<happy_proxy::server_details> servers_;
      //lock when server_ change or when new statistics are added 
      std::mutex monitor_mutex_;
      public:
      //constructor
      local_sampler();
      //copy constructor
      local_sampler(const local_sampler& other);
      //copy assignment operator
      local_sampler& operator=(const local_sampler& other);
      //move constructor
      local_sampler(local_sampler&& other);
      //destructor
      virtual ~local_sampler();
      //add server to the local sampler
      void add_server(const server_details& server);
      //remove server from the local sampler
      void remove_server(const server_details& server);
      //update server in the local sampler
      void update_server(const server_details& server);
      //get the servers from the local sampler
      std::vector<server_details> get_servers()const;
      //set the servers for the local sampler
      void set_servers(std::vector<server_details> servers);
      //get the servers from the local sampler
    };

    } //load_balancer

} //happy_proxy
#endif 