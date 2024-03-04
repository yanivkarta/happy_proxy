#include "sampler/sampler.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>
#include "proxy_util.h"
#include "proxy_server.h"
#include "proxy_session_manager.h"
#include "proxy_config.h"
#include "proxy.h"
#include "proxy_ev_queues.h"
#include "aio/aio_control.h"
#include "conntrack/iptables_helper.h"


namespace happy_proxy
{
    namespace load_balancer
    {


        //base class  sampler
         
        sampler::sampler():created_(std::chrono::system_clock::now()), last_sampled_(std::chrono::system_clock::now()), last_analyzed_(std::chrono::system_clock::now()), statistics_()

        {
            //constructor
            //initialize sampler
        } 
        
        sampler::~sampler()
        {
            //destructor
        }
        //add statistics
        void sampler::add_statistics(int64_t timestamp, std::map<std::string, std::string> statistics)
        {
            this->statistics_[timestamp] = statistics;
        }
        //get statistics
        std::map<int64_t, std::map<std::string, std::string>> sampler::get_statistics()
        {
            return this->statistics_;
        }   
        //get last sampled time
        std::chrono::time_point<std::chrono::system_clock> sampler::get_last_sampled()
        {
            return this->last_sampled_;
        }
        //get last analyzed time
        std::chrono::time_point<std::chrono::system_clock> sampler::get_last_analyzed()
        {
            return this->last_analyzed_;
        }
        //get created time
        std::chrono::time_point<std::chrono::system_clock> sampler::get_created()
        {
            return this->created_;
        }
        //sample the statistics
        void sampler::sample_statistics()
        {
            //if callback is set, call the callback
            if(this->sample_callback_)
            {
                this->sample_callback_(*this);
            }
        }
        //analyze the statistics
        void sampler::analyze_statistics()
        {
            //if callback is set, call the callback
            if(this->analyze_callback_)
            {
                this->analyze_callback_(*this);
            }
        }
        //set the sample callback
        void sampler::set_sample_callback(std::function<size_t (sampler&)> sample_callback)
        {
            this->sample_callback_ = sample_callback;
        }
        //set the analyze callback
        void sampler::set_analyze_callback(std::function<size_t (sampler&)> analyze_callback)
        {
            this->analyze_callback_ = analyze_callback;
        }

        //sampler timer
        sampler_timer::sampler_timer():sampler(), start_time_(std::chrono::system_clock::now()), end_time_(std::chrono::system_clock::now()), time_elapsed_(0)
        {
            //constructor
        }   

        //get the statistics from the sampler

    }

}