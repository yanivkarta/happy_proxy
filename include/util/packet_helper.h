#ifndef __PACKET_HELPER_H_
#define __PACKET_HELPER_H_

#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>
#include <fstream>

#include "ip_helper.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
enum layer : int8_t
{
    PHYSICAL,
    DATA_LINK,
    NETWORK,
    TRANSPORT,
    SESSION,
    PRESENTATION,
    APPLICATION
};  

enum transport : int16_t
{   TCP,
    UDP,
    ICMP,
    IGMP,
    IGRP,
    OSPF,
    EIGRP,
    RIP,
    BGP,
    LDP,
    PPTP,
    GRE,
    ESP,
    AH,
    SCTP,
    DCCP,
    RSVP,
    VRRP,
    NHRP,
    ISAKMP,
    IKE,
    L2TP,
    RADIUS,
    TACACS,
    TACACS_PLUS,
    DIAMETER,
    DNS,
    DHCP,
    BOOTP,
    TFTP,
    FTP,
    HTTP,
    HTTPS,
    SMTP,
    POP3,
    IMAP,
    SNMP,
    SSH,
    TELNET,
    NTP,
    LDAP,
    H323,
    SIP,
    RTP,
    RTCP,
    RTSP,
    SMB,
    CIFS,
    NFS,
    AFP,
    BACNET,

    //other
    GRE_IPv4,
    GRE_IPv6,
    GRE_ETHERNET,
    GRE_MPLS,
    GRE_PPP,
    GRE_PPPoE,
    GRE_L2TP,
    GRE_VPLS,
    GRE_8021Q,
    GRE_8021QinQ,
    GRE_8021ad,
    GRE_8021ah,

    //other
    GRE_IPv4_IPv6,
    GRE_IPv6_IPv6,
    GRE_IPv6_ETHERNET,
    GRE_IPv6_MPLS,
    GRE_IPv6_PPP,
    GRE_IPv6_PPPoE,
    GRE_IPv6_L2TP,
 
    //other
    GRE_ETHERNET_IPv6,
    GRE_ETHERNET_ETHERNET,
    GRE_ETHERNET_MPLS,
    GRE_ETHERNET_PPP,
    GRE_ETHERNET_PPPoE,
    GRE_ETHERNET_L2TP,
 
};

struct packet_index {
    enum layer layer;
    size_t offset;

};
class PacketHelper
{   
    enum packet_format {
        CISCO_IOS,
        XDR,
        NETFLOW,
        SFLOW,
        IPFIX,
        NETSTREAM,
        PACKFLOW,
        PCAP,
        PCAPNG,
        PCAPNG_EXT,

        //other
        //AF_IB,AF_IB_ETHERNET,AF_IB_IPv4,AF_IB_IPv6,AF_IB_MPLS,AF_IB_PPP,AF_IB_PPPoE,AF_IB_L2TP,AF_IB_VPLS,AF_IB_8021Q,AF_IB_8021QinQ,AF_IB_8021ad,AF_IB_8021ah, 
        

    };

    void *packet_;
    size_t packet_size_;
    std::vector<packet_index> packet_index_;

    public:
    PacketHelper(void *packet, size_t packet_size);
    virtual ~PacketHelper();
    void add_index(enum layer layer, size_t offset);
    void remove_index(enum layer layer, size_t offset);
    //application layer
    //presentation layer
    //physical layer
    //data link layer
    //network layer
};
#endif 