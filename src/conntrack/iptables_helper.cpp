#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <mutex>
#include <thread>
#include "iptables_helper.h"
#include "proxy.h"
#include "util/exception.h"
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
namespace happy_proxy
{
    iptables_helper::iptables_helper() : _nladdr(), _socket(0)
    {
        //constructor
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

       struct nftnl_batch *batch = nftnl_batch_alloc( NFT_MSG_BATCH, 0); 
        if (!batch) 
         {
              //throw exception
              throw happy_proxy::exception("Failed to allocate batch");
         }
        
        
       	struct mnl_socket *nl;
	    struct nft_rule *r;
	    struct nlmsghdr *nlh;
	    struct mnl_nlmsg_batch *batch;
        //family can be NFPROTO_IPV4 or NFPROTO_IPV6 or NFPROTO_UNSPEC (for both) 
        char buf[MNL_SOCKET_BUFFER_SIZE];
        unsigned int portid, seq, type;
        int ret;
        struct nftnl_rule *rule;
        struct nftnl_expr *expr;
        struct nftnl_chain *chain;
        struct nftnl_table *table;
        struct nftnl_set *set;
        struct nftnl_udata *udata;
        struct nftnl_batch *batch;
        struct nftnl_expr *expr;
        
        nl = mnl_socket_open(NETLINK_NETFILTER);
        if (nl == NULL) {
            perror("mnl_socket_open");
            exit(EXIT_FAILURE);
        }
        if (mnl_socket_bind(nl, 0, MNL_SOCKET_AUTOPID) < 0) {
            perror("mnl_socket_bind");
            exit(EXIT_FAILURE);
        }
        portid = mnl_socket_get_portid(nl);
        seq = time(NULL);
        batch = nftnl_batch_alloc(NFT_MSG_BATCH, 0);
        if (batch == NULL) {
            perror("nftnl_batch_alloc");
            exit(EXIT_FAILURE);
        }
        table = nftnl_table_alloc();
        nftnl_table_set_u32(table, NFTNL_TABLE_FAMILY, AF_INET);
        nftnl_table_set_str(table, NFTNL_TABLE_NAME, "filter");
        nftnl_batch_add_table(batch, table);
        chain = nftnl_chain_alloc();
        nftnl_chain_set_u32(chain, NFTNL_CHAIN_FAMILY, AF_INET);
        nftnl_chain_set_str(chain, NFTNL_CHAIN_TABLE, "filter");
        nftnl_chain_set_str(chain, NFTNL_CHAIN_NAME, "input");
        nftnl_batch_add_chain(batch, chain);
        rule = nftnl_rule_alloc();
        nftnl_rule_set_u32(rule, NFTNL_RULE_FAMILY, AF_INET);
        nftnl_rule_set_str(rule, NFTNL_RULE_TABLE, "filter");
        nftnl_rule_set_str(rule, NFTNL_RULE_CHAIN, "input");
        nftnl_rule_set_u32(rule, NFTNL_RULE_POSITION, 0);
        nftnl_batch_add_rule(batch, rule);
        expr = nftnl_expr_alloc("counter");
        nftnl_expr_set_u32(expr, NFTNL_EXPR_FAMILY, AF_INET);
        nftnl_expr_set_str(expr, NFTNL_EXPR_TABLE, "filter");

        nftnl_expr_set_str(expr, NFTNL_EXPR_CHAIN, "input");
        nftnl_expr_set_u32(expr, NFTNL_EXPR_POSITION, 0);
        nftnl_batch_add_expr(batch, expr);
        ret = nftnl_batch_netlink_send(batch, nl, portid, seq, 0);
        if (ret < 0) {
            perror("nftnl_batch_netlink_send");
            exit(EXIT_FAILURE);
        }
        nftnl_batch_free(batch);
        mnl_socket_close(nl);
            

 
    }

    iptables_helper::~iptables_helper()
    {
        //destructor
    }

    int iptables_helper::add_iptables_rule(const char* table, const char* chain, const char* rule)
    {
        //add iptables rule
        return 0;
    }

    int iptables_helper::delete_iptables_rule(const char* table, const char* chain, const char* rule)
    {
        //delete iptables rule
        return 0;
    }

    int iptables_helper::flush_iptables_chain(const char* table, const char* chain)
    {
        //flush iptables chain
        return 0;
    }

    int iptables_helper::create_iptables_chain(const char* table, const char* chain)
    {
        //create iptables chain
        return 0;
    }

    int iptables_helper::delete_iptables_chain(const char* table, const char* chain)
    {
        //delete iptables chain
        return 0;
    }

    int iptables_helper::list_iptables_chain(const char* table, const char* chain)
    {
        //list iptables chain
        return 0;
    }

    int iptables_helper::list_iptables_rules(const char* table, const char* chain)
    {
        //list iptables rules
        return 0;
    }

    int iptables_helper::list_iptables_tables()
    {
        //list iptables tables
        return 0;
    }

    int iptables_helper::list_iptables_chains(const char* table)
    {
        //list iptables chains
        return 0;
    }

    int iptables_helper::list_iptables_targets()
    {
        //list iptables targets
        return 0;
    }

    int iptables_helper::list_iptables_extensions()
    {
        //list iptables extensions
        return 0;
    }

    int iptables_helper::list_iptables_counters(const char* table, const char* chain)
    {
        //list iptables counters
        return 0;
    }

    int iptables_helper::zero_iptables_counters(const char* table, const char* chain)
    {
        //zero iptables counters
        return 0;
    }

    int iptables_helper::   rename_iptables_chain(const char* table, const char* old_chain, const char* new_chain)
    {
        //rename iptables chain
        return 0;
    }   

    
}