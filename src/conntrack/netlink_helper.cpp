
#include "conntrack/netlink_helper.h"
#include "util/exception.h"
#include "util/ip_helper.h"
#include "util/logger.h"
#include "util/exception.h"
#include "util/ip_helper.h"
#include "util/logger.h"
#include "util/exception.h"

//for iptables_append_entry : 
#include <linux/netfilter_ipv4/ip_tables.h> 
#include <linux/netfilter_ipv6/ip6_tables.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/nfnetlink.h>
#include <linux/netfilter/nfnetlink_log.h>
//libnftnl


#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stddef.h>	/* for offsetof */
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#include <linux/netfilter.h>
#include <linux/netfilter/nfnetlink.h>
#include <linux/netfilter/nf_tables.h>

#include <libmnl/libmnl.h>
#include <libnftnl/rule.h>
#include <libnftnl/expr.h>

namespace happy_proxy
{
    
//constructor
netlink_helper::netlink_helper()
{
    //constructor
    //initialize netlink helper
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
    //initialize the interfaces information:
    happy_proxy::interface_info info();
     //interfaces :
    std::map<std::string, std::string> interfaces = info.get_interfaces(); 
    //default gateways (get device name from interfaces map) : 
    std::map<std::string, std::string> gateways;
    for (auto it = interfaces.begin(); it != interfaces.end(); ++it)
    {
        std::string dev = it->second;
        std::string gw = info.get_default_gw(dev);
        gateways[dev] = gw;
    }
    //initialize the netlink handle
    //get initial information per interface and place in maps:
    //initialize the structs :
    struct nlmsghdr *nlh;
    struct sockaddr_nl nladdr;
    struct iovec iov;
    nladdr = (struct sockaddr_nl *)malloc(sizeof(struct sockaddr_nl));
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
    memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
    nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;
    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    memset(&nladdr, 0, sizeof(nladdr));
    nladdr.nl_family = AF_NETLINK;
    nladdr.nl_pid = 0; // For Linux Kernel
    nladdr.nl_groups = 0; // unicast
    //send the request to the kernel
    ssize_t err  = sendmsg(_socket, &msg, 0);
    if (err < 0) 
    {
        //throw exception
        throw happy_proxy::exception("Failed to send netlink request");
    }
    //receive the response from the kernel
    err = recvmsg(_socket, &msg, 0);
    if (err < 0) 
    {
        //throw exception
        throw happy_proxy::exception("Failed to receive netlink response");
    }
    




    //connect the socket and fill the nl structs 
    memset(&_nladdr, 0, sizeof(_nladdr));
    _nladdr.nl_family = AF_NETLINK;
    _nladdr.nl_pid = getpid();
    _nladdr.nl_groups = 0;

    if (bind(_socket, (struct sockaddr *)&_nladdr, sizeof(_nladdr)) < 0) 
    {
        //throw exception
        throw happy_proxy::exception("Failed to bind netlink handle");
    
    }
    //initialize the netlink handle
    //get initial information per interface and place in maps:
    //initialize the structs : 
    //struct nlmsghdr *nlh;
    //struct sockaddr_nl nladdr;
    //struct iovec iov;
    nladdr = (struct sockaddr_nl *)malloc(sizeof(struct sockaddr_nl));
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD)); 
    memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD)); 
    nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;
    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    memset(&nladdr, 0, sizeof(nladdr));
    nladdr.nl_family = AF_NETLINK;
    nladdr.nl_pid = 0; // For Linux Kernel
    nladdr.nl_groups = 0; // unicast
    //send the request to the kernel
    ssize_t err  = sendmsg(_socket, &msg, 0);
    if (err < 0) 
    {
        //throw exception
        throw happy_proxy::exception("Failed to send netlink request");
    }
    //receive the response from the kernel
    err = recvmsg(_socket, &msg, 0);
    if (err < 0) 
    {
        //throw exception
        throw happy_proxy::exception("Failed to receive netlink response");
    }
    //parse the response and fill the maps
     nlh->nlmsg_type = RTM_GETADDR;
    nlh->nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
    nlh->nlmsg_seq = 1;
    nlh->nlmsg_pid = getpid();
    //send the request to the kernel
    err = sendmsg(_socket, &msg, 0);
    if (err < 0) 
    {
        //throw exception
        throw happy_proxy::exception("Failed to send netlink request");
    }
    //receive the response from the kernel
    err = recvmsg(_socket, &msg, 0);
    if (err < 0) 
    {
        //throw exception
        throw happy_proxy::exception("Failed to receive netlink response");
    }
    //parse the response and fill the maps
    //initialize the netlink handle
 }
 
//destructor
netlink_helper::~netlink_helper()
{
    //destructor
}   

//add netlink rule
int netlink_helper::add_netlink_rule(const char* table, const char* chain, const char* rule)
{
 
    //add netlink rule
    return 0;
}



}//namespace happy_proxy