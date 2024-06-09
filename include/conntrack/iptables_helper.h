#ifndef IPTABLES_HELPER_H_
#define IPTABLES_HELPER_H_

 
#include <asm/types.h>
#include <sys/socket.h>
#include <stdint.h>
#include <linux/netfilter/nfnetlink.h>  
//for IP address :
#include <netinet/in.h>
#include <arpa/inet.h>
#include "util/ip_helper.h"
#include "util/interface_info.h"
//add policies

#include <linux/netlink.h>
#include <linux/netfilter.h>
//NFT_MSG_BATCH
#define NFT_MSG_BATCH 0x00000001 

#include <linux/netfilter/nf_tables.h>

//add iptables helper

namespace happy_proxy 
{
    //google's connman compatible iptables helper 
struct firewall_handle {
	uint64_t handle;
	const char *chain;
};
struct firewall_context {
	struct firewall_handle rule;
};
struct nftables_info {
	struct firewall_handle ct;
};

enum callback_return_type {
        CALLBACK_RETURN_NONE = 0,
        CALLBACK_RETURN_HANDLE,
        CALLBACK_RETURN_BYTE_COUNTER,
        _CALLBACK_RETURN_MAX,
};
struct callback_data {
        enum callback_return_type type;
        uint64_t value;
        bool success;
};


    class iptables_helper
    {
        //iptables helper class
        //this class is used to manage iptables rules 
        //and chains
        //it uses libnftnl to manage iptables rules
        //callbacks : 
        //events_cb
        //send_and_dispatch
        //put_batch_headers

        
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
        public:
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
        static int events_cb(const struct nlmsghdr *nlh, void *data);
        static int send_and_dispatch(struct mnl_socket *nl, const void *req,
                size_t req_size, enum callback_return_type callback_type,
                uint64_t *callback_value);
        static void put_batch_headers(char *buf, uint16_t type, uint32_t seq);
        static int add_payload(struct nftnl_rule *rule, uint32_t base,
			uint32_t dreg, uint32_t offset, uint32_t len);   
        static int add_bitwise(struct nftnl_rule *rule, int reg, const void *mask,
			size_t len);
        static int chain_cmd(struct mnl_socket *nl, struct nftnl_chain *chain,  uint16_t cmd, int family, uint16_t type,
		enum callback_return_type cb_type, uint64_t *cb_val );
        static int rule_cmd(struct mnl_socket *nl, struct nftnl_rule *rule,
			uint16_t cmd, uint16_t family, uint16_t type,
			enum callback_return_type callback_type,
			uint64_t *callback_value);
        static int rule_delete(struct firewall_handle *handle);
        struct firewall_context *__connman_firewall_create(void) ;
        void __connman_firewall_destroy(struct firewall_context *ctx); 
        static int build_rule_nat(const char *address, unsigned char prefixlen,
				const char *interface, struct nftnl_rule **res);
        //ipv4:
        static int build_rule_nat_ipv4(const char *address, unsigned char prefixlen, const char *interface, struct nftnl_rule **res); 
        //ipv6:
        static int build_rule_nat_ipv6(const char *address, unsigned char prefixlen, const char *interface, struct nftnl_rule **res); 
        //snat:

        static int build_rule_snat(int index, const char *address,
				struct nftnl_rule **res);
        
        //snat ipv4:
        static int build_rule_snat_ipv4(int index, const char *address, struct nftnl_rule **res); 
        //snat ipv6:
        static int build_rule_snat_ipv6(int index, const char *address, struct nftnl_rule **res);
        //dnat:
        static int build_rule_dnat(int index, const char *address, struct nftnl_rule **res); 
        //dnat ipv4:
        static int build_rule_dnat_ipv4(int index, const char *address, struct nftnl_rule **res);   
        //dnat ipv6:
        static int build_rule_dnat_ipv6(int index, const char *address, struct nftnl_rule **res); 
        //add rule to batch
        static int add_rule_to_batch(struct nftnl_rule *rule, struct nftnl_batch *batch); 
        //rule marking: 
        static int build_rule_marking(uid_t uid, uint32_t mark, struct nftnl_rule **res);
        //src ip: 
        static int build_rule_src_ip(const char *src_ip, uint32_t mark, struct nftnl_rule **res);
        //dst ip:
        static int build_rule_dst_ip(const char *dst_ip, uint32_t mark, struct nftnl_rule **res); 
        //build table : 
        static struct nftnl_table *build_table(const char *name, uint16_t family);
        //build chain:
        static struct nftnl_chain *build_chain(const char *name, const char *table,
				const char *type, int hooknum, int prio);
        static int create_table_and_chains(struct nftables_info *nft_info);
        static int cleanup_table_and_chains(void) ;

        //add rule to batch
        int __connman_firewall_enable_nat(struct firewall_context *ctx,
					char *address, unsigned char prefixlen,
					char *interface) ;


        int __connman_firewall_enable_snat(struct firewall_context *ctx,
				int index, const char *ifname, const char *addr);
        
        int __connman_firewall_enable_dnat(struct firewall_context *ctx, 
                int index, const char *addr); 
        
        int __connman_firewall_disable_nat(struct firewall_context *ctx);

        int __connman_firewall_disable_snat(struct firewall_context *ctx) ;


        int __connman_firewall_init(void);
        int __connman_firewall_cleanup(void); 

        iptables_helper();
        virtual ~iptables_helper();
        //load all iptables rules place in maps
        int load_iptables_rules();
 
    };//end class iptables_helper


}
#endif