#ifndef __NETLINK_HELPER_H_
#define __NETLINK_HELPER_H_

#include <memory.h>
#include <iostream>
#include <thread>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>
#include <thread>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/rtnetlink.h>
#include <unistd.h>
#include <net/if.h>
#include <linux/lwtunnel.h>
#include <linux/seg6.h>
#include <linux/seg6_local.h>
#include <linux/seg6_iptunnel.h>
#include <cassert>
#include <stdarg.h>

namespace happy_proxy {

 typedef struct
  {
    uint16_t type;
    std::string name;
  } nl_desc;

  const nl_desc descriptor[] =
    {
      { RTA_UNSPEC, "RTA_UNSPEC" },
      { RTA_DST, "RTA_DST" },
      { RTA_SRC, "RTA_SRC" },
      { RTA_IIF, "RTA_IIF" },
      { RTA_OIF, "RTA_OIF" },
      { RTA_GATEWAY, "RTA_GATEWAY" },
      { RTA_PRIORITY, "RTA_PRIORITY" },
      { RTA_PREFSRC, "RTA_PREFSRC" },
      { RTA_METRICS, "RTA_METRICS" },
      { RTA_MULTIPATH, "RTA_MULTIPATH" },
      { RTA_PROTOINFO, "RTA_PROTOINFO" },
      { RTA_FLOW, "RTA_FLOW" },
      { RTA_CACHEINFO, "RTA_CACHEINFO" },
      { RTA_SESSION, "RTA_SESSION" },
      { RTA_MP_ALGO, "RTA_MP_ALGO" },
      { RTA_TABLE, "RTA_TABLE" },
      { RTA_MARK, "RTA_MARK" },
      { RTA_MFC_STATS, "RTA_MFC_STATS" },
      { RTA_VIA, "RTA_VIA" },
      { RTA_NEWDST, "RTA_NEWDST" },
      { RTA_PREF, "RTA_PREF" },
      { RTA_ENCAP_TYPE, "RTA_ENCAP_TYPE" },
      { RTA_ENCAP, "RTA_ENCAP" },
      { RTA_EXPIRES, "RTA_EXPIRES" },
      { RTA_PAD, "RTA_PAD" },
      { RTA_UID, "RTA_UID" },
      { RTA_TTL_PROPAGATE, "RTA_TTL_PROPAGATE" },
      { RTA_IP_PROTO, "RTA_IP_PROTO" },
      { RTA_SPORT, "RTA_SPORT" },
      { RTA_DPORT, "RTA_DPORT" },
      { RTA_NH_ID, "RTA_NH_ID" },
      { __RTA_MAX, "RTA_MAX" } };

  class netlink_helper
  {
  public:
    bool _valid;
    int _socket;
    bool _running;
    void* _buffer;
    std::thread _thread;
    std::map<int, std::string> _interfaces;
    std::map<std::string, int> _indexes;
    std::map<int, struct address_info> _addresses;
    std::map<int, std::string> _default_gw;
    std::map<int, std::string> _neighbours;
    std::map<int, std::string> _neighbours6;
  

    //netlink handles and callbacks
    struct nlmsghdr *nlh;
    struct sockaddr_nl src_addr, dest_addr;
    struct iovec iov;
    struct msghdr msg;
    struct nlmsghdr *nlmsg;
    struct rtmsg *rtm;
    struct rtattr *rtattr;
    struct ifinfomsg *ifi;
    struct ifaddrmsg *ifa;
    struct rtattr **rtas;
    
    struct rtattr *rta_tb[RTA_MAX + 1];
    struct rtattr *rta_tb6[RTA_MAX + 1];
    struct rtattr *rta_tb_neigh[NDA_MAX + 1];
    struct rtattr *rta_tb_neigh6[NDA_MAX + 1];
    struct rtattr *rta_tb_neigh6_lladdr[NDA_MAX + 1];
    struct rtattr *rta_tb_neigh_lladdr[NDA_MAX + 1];
    struct rtattr *rta_tb_neigh6_cacheinfo[NDA_MAX + 1];
    struct rtattr *rta_tb_neigh_cacheinfo[NDA_MAX + 1];
    struct rtattr *rta_tb_neigh6_probes[NDA_MAX + 1];
    struct rtattr *rta_tb_neigh_probes[NDA_MAX + 1];

    struct rtattr *rta_tb_neigh6_stats[NDA_MAX + 1];
    struct rtattr *rta_tb_neigh_stats[NDA_MAX + 1];
    struct rtattr *rta_tb_neigh6_flags[NDA_MAX + 1];
    struct rtattr *rta_tb_neigh_flags[NDA_MAX + 1];
    struct rtattr *rta_tb_neigh6_vlan[NDA_MAX + 1];
    struct rtattr *rta_tb_neigh_vlan[NDA_MAX + 1];
    struct rtattr *rta_tb_neigh6_vni[NDA_MAX + 1];
    struct rtattr *rta_tb_neigh_vni[NDA_MAX + 1];
    struct rtattr *rta_tb_neigh6_master[NDA_MAX + 1];
    struct rtattr *rta_tb_neigh_master[NDA_MAX + 1];
    struct rtattr *rta_tb_neigh6_ifindex[NDA_MAX + 1];
    struct rtattr *rta_tb_neigh_ifindex[NDA_MAX + 1];
    struct rtattr *rta_tb_neigh6_lladdr_src[NDA_MAX + 1];
    struct rtattr *rta_tb_neigh_lladdr_src[NDA_MAX + 1];
    struct rtattr *rta_tb_neigh6_protocol[NDA_MAX + 1];
    struct rtattr *rta_tb_neigh_protocol[NDA_MAX + 1];
    struct rtattr *rta_tb_neigh6_table[NDA_MAX + 1];
  public:
    explicit
    netlink_helper ()  ;
    ~netlink_helper ()  ;
    int process_netlink_msg (struct sockaddr_nl *net, struct nlmsghdr *msg);
        inline std::string
    rtmsg_rtattr_summary (const struct rtattr *rta);
    inline std::string rtmsg_summary (struct nlmsghdr *nlh); 
    inline std::string
    rtnl_route_summary (const struct nlmsghdr *hdr);
    inline bool is_valid () const{ return _valid; }
    inline int get_socket () const{ return _socket; }
    inline bool is_running () const{ return _running; }
    inline void set_running (bool running){ _running = running; }
    //thread callback function - netdetect_thread
    void netdetect_thread ();
    //helpers :
     inline static std::string
    strfmt (const char *fmt, ...)
    {
      char str[1000];
      va_list args;
      va_start(args, fmt);
      vsprintf (str, fmt, args);
      va_end(args);
      return str;
    }
    inline static std::string
    inetpton (const void *ptr, int afi)
    {
      char buf[256];
      memset (buf, 0, sizeof(buf));
      inet_ntop (afi, ptr, buf, sizeof(buf));
      return buf;
    }

    inline static std::string
    ifindex2str (uint32_t ifindex)
    {
      char str[IF_NAMESIZE];
      char *ret = if_indextoname (ifindex, str);
      return ret ? ret : "none";
    }

    inline static size_t
    rta_payload (const struct rtattr *rta)
    {
      return rta->rta_len - sizeof(struct rtattr);
    }

    inline static uint8_t*
    rtattr_payload_ptr (const struct rtattr *rta)
    {
      return (uint8_t*) (rta + 1);
    }

    inline static const char*
    SEG6_IPTUNNEL_to_str (uint16_t type)
    {
      switch (type)
	{
	case SEG6_IPTUNNEL_UNSPEC:
	  return "SEG6_IPTUNNEL_UNSPEC";
	case SEG6_IPTUNNEL_SRH:
	  return "SEG6_IPTUNNEL_SRH";
	default:
	  return "SEG6_IPTUNNEL_UNKNOWN";
	}
    }
    inline static const char*
    SEG6_IPTUN_MODE_to_str (uint16_t type)
    {
      switch (type)
	{
	case SEG6_IPTUN_MODE_INLINE:
	  return "SEG6_IPTUN_MODE_INLINE";
	case SEG6_IPTUN_MODE_ENCAP:
	  return "SEG6_IPTUN_MODE_ENCAP";
	case SEG6_IPTUN_MODE_L2ENCAP:
	  return "SEG6_IPTUN_MODE_L2ENCAP";
	default:
	  return "SEG6_IPTUN_MODE_UNKNOWN";
	}
    }

    inline static std::string
    rtmsg_rtattr_SEG6_summary (const struct rtattr *rta)
    {
      std::string hdr = strfmt ("0x%04x %-20s :: ", rta->rta_type,
				SEG6_IPTUNNEL_to_str (rta->rta_type));
      switch (rta->rta_type)
	{
	case SEG6_IPTUNNEL_SRH:
	  {
	    const struct seg6_iptunnel_encap *sie;
	    sie = (const struct seg6_iptunnel_encap*) (rta + 1);
	    return hdr
		+ strfmt ("mode=%u (%s) ", sie->mode,
			  SEG6_IPTUN_MODE_to_str (sie->mode))
		+ ipv6_sr_hdr_summary (sie->srh);
	  }
	default:
	  {
	    std::string val;
	    val = strfmt ("unknown-fmt(rta_len=%u,data=", rta->rta_len);
	    const uint8_t *data = (const uint8_t*) (rta + 1);
	    size_t payload_len = size_t (rta->rta_len - sizeof(*rta));
	    size_t n = std::min (size_t (4), payload_len);
	    for (size_t i = 0; i < n; i++)
	      val += strfmt ("%02x", data[i]);
	    if (4 < payload_len)
	      val += "...";
	    val += ")";
	    return hdr + val;
	  }
	}
      return hdr + __func__;
    }

    inline static std::string
    ipv6_sr_hdr_summary (const struct ipv6_sr_hdr *srh)
    {
      std::string str = strfmt ("nh=%u hl=%u t=%u sl=%u [", srh->nexthdr,
				srh->hdrlen, srh->type, srh->segments_left,
				srh->first_segment);
      const size_t n = ((srh->hdrlen * 8 + 8) - 8) / 16;
      for (size_t i = 0; i < n; i++)
	{
	  const struct in6_addr *addr = &srh->segments[i];
	  str += strfmt ("%s%s", inetpton (addr, AF_INET6).c_str (),
			 i + 1 < n ? "," : "]");
	}
      return str;
    }

    inline std::string
    rtmsg_rtattr_summary (const struct rtattr *rta)
    {
      std::string hdr = strfmt (
	  "0x%04x %-16s :: ",
	  rta->rta_type,
	  ((rta->rta_type < __RTA_MAX) ?
	      descriptor[rta->rta_type].name.c_str () :
	      descriptor[__RTAX_MAX].name.c_str ()));

      std::string val;
      val = strfmt (" (rta_len=%u,data=", rta->rta_len);
      const uint8_t *data = (const uint8_t*) (rta + 1);
      size_t payload_len = size_t (rta->rta_len - sizeof(*rta));
      size_t n = std::min (size_t (4), payload_len);
      for (size_t i = 0; i < n; i++)
	val += strfmt ("%02x", data[i]);
      if (4 < payload_len)
	val += "...";
      val += ")";
      return hdr + val;

    }

    inline std::string
    rtnl_route_summary (const struct nlmsghdr *hdr)
    {
      struct rtmsg *rtm = (struct rtmsg*) (hdr + 1);
      std::string str = rtmsg_summary (rtm) + "\n";
      size_t rta_len = IFA_PAYLOAD(hdr);
      for (struct rtattr *rta = RTM_RTA(rtm); RTA_OK(rta, rta_len); rta =
	  RTA_NEXT(rta, rta_len))
	{
	  std::string attr_str = rtmsg_rtattr_summary (rta);
	  if (attr_str != "")
	    str += "  " + attr_str + "\n";
	}
      return str;
    }//rtnl_route_summary
  };//netlink_helper
}//happy_proxy
#endif