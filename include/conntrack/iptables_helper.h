#ifndef IPTABLES_HELPER_H_
#define IPTABLES_HELPER_H_

 

#include <stdint.h>
#include <linux/netfilter.h>
#include <linux/netfilter/nfnetlink.h>  
//for IP address :
#include <netinet/in.h>
#include <arpa/inet.h>
#include "util/ip_helper.h"
#include "util/interface_info.h"
//add policies

//add iptables helper

namespace happy_proxy 
{
    
    class iptables_helper
    {
        //iptables helper class
        //this class is used to manage iptables rules 
        //and chains
        //it uses libnftnl to manage iptables rules
        
        sockaddr_nl _nladdr;
        int _socket;
        std::map<std::string, std::string> _tables;
        std::map<std::string, std::string> _chains;
        std::map<std::string, std::string> _rules;
        std::map<std::string, std::string> _targets;
        std::map<std::string, std::string> _extensions;
        std::map<std::string, std::string> _counters;
        std::map<std::string, std::string> _policies;
        std::map<std::string, std::string> _tables_chains;

        //add iptables rule
        int add_iptables_rule(const char* table, const char* chain, const char* rule);
        //delete iptables rule
        int delete_iptables_rule(const char* table, const char* chain, const char* rule);
        //flush iptables chain
        int flush_iptables_chain(const char* table, const char* chain);
        //create iptables chain
        int create_iptables_chain(const char* table, const char* chain);
        //delete iptables chain
        int delete_iptables_chain(const char* table, const char* chain);
        //list iptables chain
        int list_iptables_chain(const char* table, const char* chain);
        //list iptables rules
        int list_iptables_rules(const char* table, const char* chain);
        //list iptables tables
        int list_iptables_tables();
        //list nfnetqueue targets
        int list_iptables_targets();
        //list iptables extensions
        int list_iptables_extensions();
        //list iptables counters
        int list_iptables_counters(const char* table, const char* chain); 
        //zero iptables counters
        int zero_iptables_counters(const char* table, const char* chain);
        //rename iptables chain
        int rename_iptables_chain(const char* table, const char* old_chain, const char* new_chain);
        //set iptables policy
        int policy_iptables_chain(const char* table, const char* chain, const char* policy); 
        

        public:
        iptables_helper();
        ~iptables_helper();
        //load all iptables rules place in maps
        int load_iptables_rules();

        int add_iptables_rule(const char* table, const char* chain, const char* rule); 
        int delete_iptables_rule(const char* table, const char* chain, const char* rule); 
        int flush_iptables_chain(const char* table, const char* chain);
        int create_iptables_chain(const char* table, const char* chain);
        int delete_iptables_chain(const char* table, const char* chain);
        int list_iptables_chain(const char* table, const char* chain);
        int list_iptables_rules(const char* table, const char* chain);
        int list_iptables_tables();
        int list_iptables_chains(const char* table);
        int list_iptables_targets();
        int list_iptables_extensions();
        int list_iptables_counters(const char* table, const char* chain);
        int zero_iptables_counters(const char* table, const char* chain);
        int rename_iptables_chain(const char* table, const char* old_chain, const char* new_chain);
        int policy_iptables_chain(const char* table, const char* chain, const char* policy);
        int insert_iptables_rule(const char* table, const char* chain, const char* rule, int pos);
        int append_iptables_rule(const char* table, const char* chain, const char* rule);
        int replace_iptables_rule(const char* table, const char* chain, const char* rule, int pos);
        int check_iptables_rule(const char* table, const char* chain, const char* rule);
        int check_iptables_chain(const char* table, const char* chain);
        int check_iptables_policy(const char* table, const char* chain, const char* policy);
        int check_iptables_target(const char* table, const char* target);
        int check_iptables_extension(const char* table, const char* extension);
        int check_iptables_counter(const char* table, const char* chain);
        int check_iptables_table(const char* table);
        int check_iptables_chain(const char* table, const char* chain);
        int check_iptables_rule(const char* table, const char* chain, const char* rule);

    };

}