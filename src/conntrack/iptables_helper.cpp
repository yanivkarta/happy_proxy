#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <mutex>
#include <thread>
#include "proxy.h"
#include "util/exception.h"
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <libnftnl/gen.h>
#include <libnftnl/table.h>
#include <libnftnl/chain.h>
#include <libnftnl/rule.h>
#include <libnftnl/expr.h>
#include <libnftnl/set.h>
#include <libnftnl/udata.h>
#include <libnftnl/batch.h>
//NF_NETLINK_CONNTRACK_ERROR
#include <linux/netfilter/nfnetlink_conntrack.h>
#include <linux/netfilter/nfnetlink_log.h>
#include <linux/netfilter/nfnetlink_queue.h>
#include <linux/netfilter/nfnetlink.h>
#include <linux/netfilter/nf_tables.h>
#include <linux/netfilter/nfnetlink_conntrack.h>
#include <linux/netfilter/nfnetlink_log.h>
#include <linux/netfilter/nfnetlink_queue.h>
#include <linux/netfilter/nfnetlink.h>
#include <linux/netfilter/nf_tables.h>
#include <libmnl/libmnl.h>
#include <libnftnl/rule.h>
#include <libnftnl/expr.h>
#include <libnftnl/chain.h>
#include <libnftnl/table.h>
#include <libnftnl/batch.h>
#include <libnftnl/set.h>
#include <libnftnl/udata.h>

#include "iptables_helper.h"

#ifndef MNL_SOCKET_BUFFER_SIZE
#define MNL_SOCKET_BUFFER_SIZE 32768 
#endif
#ifndef MAX_PAYLOAD
#define MAX_PAYLOAD 4096
#endif

//for NFTNL_EXPR_FAMILY,
#include <libnftnl/expr.h>
//for NFTNL_EXPR_TABLE,
#include <libnftnl/batch.h>



namespace happy_proxy
{

 

    //c++ implementation of firewall-nftables.c
    //https://kernel.googlesource.com/pub/scm/network/connman/connman/+/refs/heads/master/src/firewall-nftables.c
    //

    iptables_helper::iptables_helper() : _nladdr(), _socket(0)
    {
        //constructor
        //initialize iptables helper
        //initialize netlink handle
        _socket = socket(AF_NETLINK, SOCK_RAW, NETLINK_NETFILTER); 
        if (_socket < 0) 
        {
            //throw exception
            throw happy_proxy::exception("Failed to open netlink handle");
        }
        //check permissions
        if (geteuid() != 0) 
        {
            //throw exception
            throw happy_proxy::exception("You must be root to use netlink");
        }
        //enumerate iptables rules,tables,chains 
        //and set the iptables rules
        //to the iptables rules in the system 



        
    }//end of constructor
    
    //google's compatible connman iptables helper 

    iptables_helper::~iptables_helper()
    {
 
        //destructor
        close(_socket); 
    }

    int iptables_helper::add_iptables_rule(const char* table, const char* chain, const char* rule)
    {
        //add iptables rule
        //get table and chain:
        struct nftnl_table *t = nftnl_table_alloc(); 
        if (t == NULL) {
            perror("nftnl_table_alloc");
            exit(EXIT_FAILURE);
        }
        //set 'table' name 
        nftnl_table_set_str(t, NFTNL_TABLE_NAME, table); 

        struct nftnl_chain *c = nftnl_chain_alloc();
        if (c == NULL) {
            perror("nftnl_chain_alloc");
            exit(EXIT_FAILURE);
        }
        //set chain: 
        nftnl_chain_set_str(c, NFTNL_CHAIN_TABLE, chain); 

        //set table name
        //add rule to chain 
        //allocate rule 
        struct nftnl_rule *r = nftnl_rule_alloc(); 
        if (r == NULL) {
            perror("nftnl_rule_alloc");
            exit(EXIT_FAILURE);
        }
        //set rule 
        nftnl_rule_set_str(r, NFTNL_RULE_CHAIN, rule);
        //add rule to chain 
        nftnl_chain_rule_add_tail(r,c); 
        //free rule
        nftnl_rule_free(r);
        return 0;
        //return 0 on success

    }//end of add_iptables_rule


    int iptables_helper::delete_iptables_rule(const char* table, const char* chain, const char* rule)
    {
        //get table, chain and rule 
        struct nftnl_table *t = nftnl_table_alloc(); 
        if (t == NULL) {
            perror("nftnl_table_alloc");
            exit(EXIT_FAILURE);
        }
        //set 'table' name
        nftnl_table_set_str(t, NFTNL_TABLE_NAME, table);
        
        //get the rules and chain matching the string in the table
        struct nftnl_chain *c = nftnl_chain_alloc(); 
        if (c == NULL) {
            perror("nftnl_chain_alloc");
            exit(EXIT_FAILURE);
        }
        //set chain
        nftnl_chain_set_str(c, NFTNL_CHAIN_TABLE, chain);
        //get the rule
        struct nftnl_rule *r = nftnl_rule_alloc(); 
        if (r == NULL) {
            perror("nftnl_rule_alloc");
            exit(EXIT_FAILURE);
        } 
        //set rule 
        nftnl_rule_set_str(r, NFTNL_RULE_CHAIN, rule); 
        //find rule in chain
        //delete rule from chain
        nftnl_chain_rule_del(r);
        //free rule
        nftnl_rule_free(r);
        return 0;

        
    }//end of delete_iptables_rule

    int iptables_helper::flush_iptables_chain(const char* table, const char* chain)
    {
        //flush iptables chain:
        struct nftnl_table *t = nftnl_table_alloc(); 
        if (t == NULL) {
            perror("nftnl_table_alloc");
            exit(EXIT_FAILURE);
        } 
        //set 'table' name 
        nftnl_table_set_str(t, NFTNL_TABLE_NAME, table); 
        //get chain
        struct nftnl_chain *c = nftnl_chain_alloc(); 
        if (c == NULL) {
            perror("nftnl_chain_alloc");
            exit(EXIT_FAILURE);
        } 
        //set chain 
        nftnl_chain_set_str(c, NFTNL_CHAIN_TABLE, chain); 
        //flush chain 
        
        //get the rules from the chain 
        struct nftnl_rule *r = nftnl_rule_alloc(); 
        if (r == NULL) {
            perror("nftnl_rule_alloc");
            exit(EXIT_FAILURE);
        }
        //set rule
        nftnl_rule_set_str(r, NFTNL_RULE_CHAIN, chain);
        //delete rule from chain

        nftnl_chain_rule_del(r); 
        return 0; 
    }
    
    int iptables_helper::create_iptables_chain(const char* table, const char* chain)
    {
        //create new iptables chain 
        struct nftnl_table *t = nftnl_table_alloc(); 
        if (t == NULL) {
            perror("nftnl_table_alloc");
            exit(EXIT_FAILURE);
        }
        //set 'table' name 
        nftnl_table_set_str(t, NFTNL_TABLE_NAME, table); 
        //create chain 
        struct nftnl_chain *c = nftnl_chain_alloc(); 
        if (c == NULL) {
            perror("nftnl_chain_alloc");
            exit(EXIT_FAILURE);
        } 
        //set chain 
        nftnl_chain_set_str(c, NFTNL_CHAIN_TABLE, chain); 
        return 0; 

    }

    int iptables_helper::delete_iptables_chain(const char* table, const char* chain)
    {
 
        //find and delete iptables chain
        struct nftnl_table *t = nftnl_table_alloc(); 
        if (t == NULL) {
            perror("nftnl_table_alloc");
            exit(EXIT_FAILURE);
        }
        //set 'table' name 
        nftnl_table_set_str(t, NFTNL_TABLE_NAME, table); 
        //get chain
        struct nftnl_chain *c = nftnl_chain_alloc(); 
        if (c == NULL) {
            perror("nftnl_chain_alloc");
            exit(EXIT_FAILURE);
        }
        //set chain
        nftnl_chain_set_str(c, NFTNL_CHAIN_TABLE, chain); 
        //delete chain
        nftnl_chain_free(c); 

         return 0;
    }//end of delete_iptables_chain
    

    int iptables_helper::load_iptables_rules()
    {
        //load iptables rules
        //load all iptables rules place in map
        auto init_iptables_rules_indices = []() -> std::map<std::string, size_t> {

            std::map<std::string, size_t> iptables_rules_indices;
            //get all the iptables rules
            struct nftnl_table *t = nftnl_table_alloc(); 
            if (t == NULL) {
                perror("nftnl_table_alloc");
                exit(EXIT_FAILURE);
            }   
            //set 'table' name
            nftnl_table_set_str(t, NFTNL_TABLE_NAME, "filter"); 
            //get all the chains
            struct nftnl_chain *c = nftnl_chain_alloc(); 
            if (c == NULL) {
                perror("nftnl_chain_alloc");
                exit(EXIT_FAILURE);
            } 
            //set chain
            nftnl_chain_set_str(c, NFTNL_CHAIN_TABLE, "nat"); 
            //get all the rules from the chain      
            struct nftnl_rule *r = nftnl_rule_alloc(); 
            if (r == NULL) {
                perror("nftnl_rule_alloc");
                exit(EXIT_FAILURE);
            } 
            //set rule
            nftnl_rule_set_str(r, NFTNL_RULE_CHAIN, "nat");
            //add rule to chain
            nftnl_chain_rule_add(r, c);
            //get the rule index
            size_t rule_index = nftnl_rule_get_u32(r, NFTNL_RULE_POSITION); 
            iptables_rules_indices["nat"] = rule_index;
             return iptables_rules_indices;

        }();
         for (auto& [key, value] : this->iptables_rules_indices_) {
            std::cout << "key : " << key << " value : " << value << std::endl;  

        }
        for (auto& [key, value] : init_iptables_rules_indices) {
            std::cout << "key : " << key << " value : " << value << std::endl;
        
        }
        return 0;
    }//end of load_iptables_rules 
    // __connman_firewall_enable_nat  :
    // __connman_firewall_disable_nat :
    int iptables_helper::__connman_firewall_enable_nat(struct firewall_context *ctx,
					char *address, unsigned char prefixlen,
					char *interface) 
    {
        //address,prefix,interface,context:
        //enable nat for connman firewall
    
        if(ctx==NULL)
        {
            return -1;
        }
        else{

            std::cout << "connman_firewall_enable_nat" << address<<" "<<std::to_string(prefixlen)<<" "<<interface<<" "<< std::endl; 
        }
        

        return 0;

    }
    //__connman_firewall_init:
    int iptables_helper::__connman_firewall_init(void) 
    {
        std::cout << "connman_firewall_init" << std::endl; 

        return 0;
    }
    // Print all iptables rules
    void iptables_helper::print_iptables_rules() {
        for (const auto& rule : iptables_rules_indices_) {
            std::cout << "Rule: " << rule.first << " Index: " << rule.second << std::endl;
        }
    }

    //end of class iptables_helper

}//end of namespace happy_proxy