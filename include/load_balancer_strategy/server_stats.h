#ifndef __SERVER_STATS_H_
#define __SERVER_STATS_H_

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


namespace happy_proxy
{
  namespace load_balancer
  {
    struct server_stats
    {
      //statics:
      std::string name;
      std::string description; 
      std::string type; //web, app, db, cache, storage, network, security, monitoring, logging, analytics, iot, ai, ml, dl, bigdata, cloud, edge, fog, mobile, desktop, server, container, vm, os, hardware, software, firmware, middleware, api, sdk, library, framework, platform, service, system, solution, application, tool, utility,
      std::string version; //major.minor.patch
      std::string author; //company, organization, individual
      std::string license; //   MIT, Apache, GPL, LGPL, BSD, Proprietary, Commercial, Open Source, Free Software, Public Domain, Creative Commons, Copyleft, Permissive, Share Alike, 
      std::string date; //YYYY-MM-DD
      std::string time; //HH:MM:SS
      std::string comments; //  description, notes, remarks, observations,
      std::string server_name; //hostname
      std::string server_ip; //ipv4 or ipv6
      std::string server_port; //port number
      std::string server_status; //active, inactive, standby, maintenance, retired, 
      std::string server_type; //web, app, db, cache, storage, network, security, monitoring, logging, analytics, iot, ai, ml, dl, bigdata, cloud, edge, fog, mobile, desktop, server, container, vm, os, hardware, software, firmware, middleware, api, sdk, library, framework, platform, service, system, solution, application, tool, utility, 
      std::string server_version; //major.minor.patch
      std::string server_os;  //os name 
      std::string server_cpu; //cpu name
      std::string server_memory; //memory size
      
      std::string server_disk; //storage size
      std::string server_network; //network environment
      std::string server_storage;//storage environment
      std::string server_cache; //cache environment
      std::string server_database;//database environment

    std::string server_security; //security environment
        
    typedef struct snmp_v2_stats 
    {
        //snmp mib-2 stats 
        std::string snmp_v2_system; //system
        std::string snmp_v2_interfaces; //interfaces
        std::string snmp_v2_ip; //ip
        std::string snmp_v2_icmp; //icmp

        std::string snmp_v2_tcp; //tcp
        std::string snmp_v2_udp; //udp
        std::string snmp_v2_snmp; //snmp
        std::string snmp_v2_host; //host

        //cpu stats
        std::string snmp_v2_cpu_user; //user
        std::string snmp_v2_cpu_system; //system
        std::string snmp_v2_cpu_idle; //idle
        std::string snmp_v2_cpu_wait; //wait

        //memory stats
        std::string snmp_v2_memory_total; //total
        std::string snmp_v2_memory_free; //free
        std::string snmp_v2_memory_buffer; //buffer
        std::string snmp_v2_memory_cached; //cached

        //storage stats
        std::string snmp_v2_storage_total; //total
        std::string snmp_v2_storage_free; //free
        std::string snmp_v2_storage_used; //used
        std::string snmp_v2_storage_avail; //avail

        //network stats
        std::string snmp_v2_network_in; //in
        std::string snmp_v2_network_out; //out
        std::string snmp_v2_network_error; //error
        std::string snmp_v2_network_collision; //collision

        //interface stats
        std::string snmp_v2_interface_in; //in
        std::string snmp_v2_interface_out; //out
        std::string snmp_v2_interface_error; //error
        std::string snmp_v2_interface_collision; //collision

        //ip stats
        std::string snmp_v2_ip_in; //in
        std::string snmp_v2_ip_out; //out
        std::string snmp_v2_ip_error; //error
        std::string snmp_v2_ip_collision; //collision

        //icmp stats
        std::string snmp_v2_icmp_in; //in
        std::string snmp_v2_icmp_out; //out
        std::string snmp_v2_icmp_error; //error
        std::string snmp_v2_icmp_collision; //collision

        //tcp stats
        std::string snmp_v2_tcp_in; //in
        std::string snmp_v2_tcp_out; //out
        std::string snmp_v2_tcp_error; //error
        std::string snmp_v2_tcp_collision; //collision
        
 
     }snmp_stats,*snmp_stats_ptr;
     snmp_stats_ptr snmp_v2_stats_ptr;

    };
    }
}



#endif 