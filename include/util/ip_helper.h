#ifndef __IP_HELPER_H__
#define __IP_HELPER_H__

#include <netinet/in.h>

#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <memory.h>

#include <cmath>
#include <cstdint>
#include <string>
#include <iostream>
#include <iosfwd>
#include <strstream>
#include <sstream>

#include <vector>
namespace happy_proxy
{


__extension__ typedef unsigned __int128 uint128_t;

 template <typename Integer = uint32_t>
    class prefix
    {
        public:
        Integer value;
        Integer mask;
        //address and netmask
      
        Integer addr;
        Integer netmask;
        
        prefix(std::string addr_str, std::string mask_str) : value(0), mask(0) {
            if (std::is_same<Integer, uint128_t>::value) 
            {
                // IPv6 Address Parsing
                addr = ipv6_to_int(addr_str);
                
                
                auto mask = mask_to_int128(std::stoi(mask_str)); 
                netmask = mask;
                
            }
            else
            {
                // IPv4 Address Parsing
                addr = ipv4_to_int(addr_str);
                netmask = mask_to_int(std::stoul(mask_str));
            }   
        }
        prefix(std::string addr_str, int mask) : value(0), mask(0) {
            if (std::is_same<Integer, uint128_t>::value)
            {
                // IPv6 Address Parsing
                addr = ipv6_to_int(addr_str);
                netmask = mask_to_int(mask);
            }
            else
            {
                // IPv4 Address Parsing
                addr = ipv4_to_int(addr_str);
                netmask = mask_to_int(mask);
            }
        }   
        prefix(Integer addr, Integer mask) : value(0), mask(0) {
            this->addr = addr;
            this->netmask = mask;
        }   
        prefix(const prefix& other) : value(other.value), mask(other.mask) {
            this->addr = other.addr;
            this->netmask = other.netmask;
        }   
        prefix& operator=(const prefix& other) {
            value = other.value;
            mask = other.mask;
            this->addr = other.addr;
            this->netmask = other.netmask;
            return *this;
        }
      

        static inline uint32_t
		ipv4_to_int(std::string addr_str)
		{
			int counter = 0;			 // Check for proper addr length
			size_t pos = 0;				 // Position in string addr
			uint32_t ipv4_ip_int = 0;	 // Stores the address as an int
			std::string token;			 // Buffer subseciton of addr
			std::string delimiter = "."; // String dilimiter
			bool error_f = false;		 // Error flag, drops malformed input

			// Create a copy of address string
			std::string s = addr_str;
			if (s.empty())
			{
				error_f = true;
			}
			else
			{
				while ((pos = s.find(delimiter)) != std::string::npos)
				{
					// Catch long malformed input
					if (counter > 3)
					{
						error_f = true;
						break;
					}
					// Token is one 8-bit int
					token = s.substr(0, pos);
					try
					{
						uint32_t token_int = std::stoul(token);
						// Catch out of range ints, default to 255
						if (token_int > 255)
						{
							error_f = true;
							break;
						}
						// Add token and shift left 8 bits
						ipv4_ip_int += token_int;
						ipv4_ip_int = ipv4_ip_int << 8;
						// Trim token from addr
						s.erase(0, pos + delimiter.length());
						counter += 1;
					}
					catch (...)
					{
						error_f = true;
						break;
					}
				}
				// Catch short malformed input
				if (counter != 3)
				{
					error_f = true;
				}
				// Add last 8-bit token
				try
				{
					uint32_t s_int = std::stoul(s);
					if (s_int > 255)
					{
						error_f = true;
					}
					else
					{
						ipv4_ip_int += s_int;
					}
				}
				catch (...)
				{
					// Catch malformed input
					// Default to
					//	
					
					error_f = true;
				}
			}
			// Default errors to 0.0.0.0
			if (error_f == true)
			{
				ipv4_ip_int = 0;
				std::cerr << "[-]Caught malformed IPv4 address: " << addr_str;
			}
			return ipv4_ip_int;
		}

		static inline uint128_t
		ipv6_to_int(std::string addr_str)
		{
			int counter = 0;			 // Check for proper addr length
			int omit_zeros_pos = -1;	 // Stores the position of ommited blocks of zeros
			size_t pos = 0;				 // Position in string addr
			uint128_t ipv6_ip_int = 0;	 // Stores the address as an int
			std::string token;			 // Buffer subseciton of addr
			std::string delimiter = ":"; // String dilimiter
			bool error_f = false;		 // Error flag, drops malformed input

			// Create a copy of address string
			std::string s = addr_str;
			if (s.empty())
			{
				error_f = true;
			}
			else
			{
				while ((pos = s.find(delimiter)) != std::string::npos)
				{
					// Catch long malformed input
					if (counter > 7)
					{
						error_f = true;
						break;
					}
					// Token is one 4-byte int
					token = s.substr(0, pos);
					try
					{
						// Encountered suppressed blocks of zeros
						if (token.size() == 0)
						{
							// Save the position
							omit_zeros_pos = counter;
							// Erase next semicolon
							s.erase(0, pos + delimiter.length());
							continue;
						}
						uint32_t token_int = std::stoi(token, 0, 16);

						// Catch out of range ints, default to 65535
						if (token_int > 65535)
						{
							error_f = true;
							break;
						}
						// Add token and shift left 16 bits
						ipv6_ip_int += token_int;
						ipv6_ip_int = ipv6_ip_int << 16;
						// Trim token from addr
						s.erase(0, pos + delimiter.length());
						counter += 1;
					}
					catch (...)
					{
						error_f = true;
						break;
					}
				}
				// Catch short malformed input
				if (counter > 7)
				{
					error_f = true;
				}

				try
				{
					// Add the last 16-bit token
					if (s.size() != 0)
					{
						uint32_t s_int = std::stoi(s, 0, 16);
						if (s_int > 65536)
						{
							error_f = true;
						}
						else
						{
							ipv6_ip_int += s_int;
						}
					}

					// Insert ommited blocks of zeros
					if (omit_zeros_pos != -1)
					{
						// Calculate the number of ommited blocks
						int omit_blocks = 7 - counter;
						// Split ipv6_ip_int into two separate numbers (split at the position of omitted blocks)
						int shift_amount = (16 * (8 - (omit_blocks + omit_zeros_pos)));
						// Shift left half right and move it back to clear the right half
						uint128_t left_half = ipv6_ip_int >> shift_amount;
						left_half = left_half
									<< (16 * (8 - (omit_blocks + omit_zeros_pos)));
						uint128_t right_half = left_half ^ ipv6_ip_int;
						// Shift left half to add zeros in place of omitted blocks
						left_half = left_half << (omit_blocks * 16);
						// Reconstruct the address from the two halves
						ipv6_ip_int = left_half | right_half;
					}
				}
				catch (...)
				{
					error_f = true;
				}
			}
			// Default errors to ::
			if (error_f == true)
			{
				ipv6_ip_int = 0;
				std::cerr << "[-]Caught malformed IPv6 address: " << addr_str;
			}
			return ipv6_ip_int;
		}
		std::string
		to_query() const
		{
			size_t nIterToR = 0;
			if (std::is_same<Integer, uint128_t>::value)
			{
				// yaniv - changed ostream to strstream:
				std::strstream query;

				for (int i = 112; i >= 0; i = i - 16)
				{
					// Process each quad separately
					int quad = (addr >> i) & 0xFFFF;
					// Convert quad to hex and add to cidr
					query << std::hex << quad;
					// Don't add a colon after the last quad
					if (i != 0)
					{
						if (++nIterToR < 3)
							query << ":";
						else
							break;
					}
				}
				query << std::dec << "%/%";
				return query.str();
			}

			else
			{
				std::string query = "";
				uint8_t quad = (addr & 0xFF000000) >> 24;
				query.append(std::to_string(quad) + ".");
				quad = (addr & 0x00FF0000) >> 16;
				query.append(std::to_string(quad) + ".");
				query.append("%.%/%");
				return query;
			}
		}
		std::string
		to_cidr() const
		{
			// Check if this prefix is IPv6 or IPv4
			if (std::is_same<Integer, uint128_t>::value)
			{
				// yaniv - changed ostream to strstream:
				std::strstream cidr;
				for (int i = 112; i >= 0; i = i - 16)
				{
					// Process each quad separately
					int quad = (addr >> i) & 0xFFFF;
					// Convert quad to hex and add to cidr
					cidr << std::hex << quad;
					// Don't add a colon after the last quad
					if (i != 0)
					{
						cidr << ":";
					}
				}
				cidr << '/';

				// Calculate the number of 1s in netmask
				uint32_t sz = 0;
				for (int i = 0; i < 128; i++)
				{
					if (netmask & ((uint128_t)1 << i))
					{
						sz++;
					}
				}
				// Switch stringstream back to decimal
				cidr << std::dec << sz;
				return cidr.str();
			}
			else
			{
				std::string cidr = "";
				uint8_t quad = (addr & 0xFF000000) >> 24;
				cidr.append(std::to_string(quad) + ".");
				quad = (addr & 0x00FF0000) >> 16;
				cidr.append(std::to_string(quad) + ".");
				quad = (addr & 0x0000FF00) >> 8;
				cidr.append(std::to_string(quad) + ".");
				quad = (addr & 0x000000FF) >> 0;
				cidr.append(std::to_string(quad));
				cidr.push_back('/');
				// Assume valid cidr netmask, e.g. no ones after the first zero
				uint8_t sz = 0;
				for (int i = 0; i < 32; i++)
				{
					if (netmask & (1 << i))
					{
						sz++;
					}
				}
				cidr.append(std::to_string(sz));
				return cidr;
			}
			return "";
		}

		/** operator<< is not defined for 128 bit integers
		 *  Those two functions convert the address and netmask to strings
		 *  They should only be used for debugging since they are pretty slow
		 */
		std::string
		addr_to_string() const
		{
			std::string str;
			uint128_t num = addr;
			do
			{
				int digit = num % 10;
				str = std::to_string(digit) + str;
				num /= 10;
			} while (num != 0);
			return str;
		}
		std::string
		netmask_to_string() const
		{
			std::string str;
			uint128_t num = netmask;
			do
			{
				int digit = num % 10;
				str = std::to_string(digit) + str;
				num /= 10;
			} while (num != 0);
			return str;
		}

		/** Defined comparison operators for maps
		 *
		 * Comparing the addr first ensures the more specific address is greater
		 *
		 * @param b The Prefix object to which this object is compared.
		 * @return true If the operation holds, otherwise false
		 */
		bool
		operator<(const prefix<Integer> &b) const
		{
			if (std::is_same<Integer, uint128_t>::value)
			{
				if (addr != b.addr)
				{
					return addr < b.addr;
				}
				else
				{
					return netmask < b.netmask;
				}
			}
			else
			{
				uint64_t combined = 0;
				combined |= addr;
				combined = combined << 32;
				combined |= netmask;
				uint64_t combined_b = 0;
				combined_b |= b.addr;
				combined_b = combined_b << 32;
				combined_b |= b.netmask;
				return combined < combined_b;
			}
        
		}
		bool
		operator==(const prefix<Integer> &b) const
		{
			return !(*this < b || b < *this);
		}
		bool
		operator>(const prefix<Integer> &b) const
		{
			return !(*this < b || *this == b);
		}
		bool
        operator<=(const prefix<Integer> &b) const
        {
            return *this < b || *this == b;
        }
		bool
		contained_in_or_equal_to(const prefix<Integer> &b) const
		{
			return b.netmask <= netmask && (addr & b.netmask) == (b.addr & b.netmask);
		}

        
           
        bool contains(Integer value) const
        {
            return (value & mask) == this->value;
        }
        bool contains(const prefix& other) const
        {
            return (other.value & mask) == this->value && (other.mask & mask) == this->mask;
        }
        bool overlaps(const prefix& other) const
        {
            return (value & other.mask) == other.value || (other.value & mask) == value;
        }
        bool is_contained(const prefix& other) const
        {
            return (value & other.mask) == other.value && (mask & other.mask) == other.mask;
        }
        bool is_contained(Integer value, Integer mask) const
        {
            return (value & mask) == this->value && (mask & mask) == this->mask;
        }
        bool is_contained(Integer value) const
        {
            return (value & mask) == this->value;
        }
        bool is_contained(const prefix& other, Integer mask) const
        {
            return (value & mask) == other.value && (mask & mask) == other.mask;
        }

        /*static inline uint32_t
		ipv4_to_int(std::string addr_str)
		{
            struct in_addr addr;
            inet_pton(AF_INET, addr_str.c_str(), &addr);
            return ntohl(addr.s_addr);
        }
        static inline std::string                    
        int_to_ipv4(uint32_t addr)
        {
            struct in_addr in;
            in.s_addr = htonl(addr);
            char str[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &in, str, INET_ADDRSTRLEN);
            return std::string(str);
        }
        static inline uint128_t
        ipv6_to_int(std::string addr_str)
        {
            struct in6_addr addr;
            inet_pton(AF_INET6, addr_str.c_str(), &addr);
            uint128_t result = 0;
            for (int i = 0; i < 16; i++)
            {
                result = (result << 8) + addr.s6_addr[i];
            }
            return result;
        }
        static inline std::string
        int_to_ipv6(uint128_t addr)
        {
            struct in6_addr in;
            for (int i = 15; i >= 0; i--)
            {
                in.s6_addr[i] = addr & 0xff;
                addr = addr >> 8;
            }
            char str[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, &in, str, INET6_ADDRSTRLEN);
            return std::string(str);
        }*/
        static inline uint32_t
        mask_to_int(int mask)
        {
            return mask == 0 ? 0 : ~((1 << (32 - mask)) - 1);
        }
        static inline uint128_t
        mask_to_int128(int mask)
        {
            return mask == 0 ? 0 : ~((uint128_t(1) << (128 - mask)) - 1);
        }
        static inline int
        int_to_mask(uint32_t mask)
        {
            if (mask == 0)
            {
                return 0;
            }
            int result = 0;
            while ((mask & 1) == 0)
            {
                mask = mask >> 1;
                result++;
            }
            return 32 - result;
        }
        static inline int
        int128_to_mask(uint128_t mask)
        {
            if (mask == 0)
            {
                return 0;
            }
            int result = 0;
            while ((mask & 1) == 0)
            {
                mask = mask >> 1;
                result++;
            }
            return 128 - result;
        }
      


    };//end of prefix class
//helper class for IPv4 and IPv6 address manipulation 
//also supports subnet masking , and IP address range checking 
    class ip_address
    {
        public:
		ip_address() : family_(AF_UNSPEC), associated_port(0)
		{
			::memset(&u_, 0, sizeof(u_));
		}


		explicit ip_address(const in_addr &ip4) : family_(AF_INET), associated_port(0)
		{
			u_.ip4 = ip4;
		}

		explicit ip_address(const in6_addr &ip6) : family_(AF_INET6), associated_port(0)
		{
			u_.ip6 = ip6;
		}
		explicit ip_address(uint32_t ip_in_host_byte_order) : family_(AF_INET), associated_port(0)
		{
			memset(&u_, 0, sizeof(u_));
			u_.ip4.s_addr = htonl(ip_in_host_byte_order);
		}

		explicit ip_address(const std::string &ip) : family_(AF_UNSPEC), associated_port(0)
		{
 
			memset(&u_, 0, sizeof(u_));

			if (ip.find_first_of(':', 0) != std::string::npos)
			{
				family_ = AF_INET6;
				uint128_t ip128 = prefix<uint128_t>::ipv6_to_int(ip.c_str());
				memcpy(&u_.ip6.__in6_u.__u6_addr8, &ip128, sizeof(uint128_t));
			}
			else if (ip.find_first_of('.', 0) != std::string::npos)
			{

				family_ = AF_INET;
				u_.ip4.s_addr = prefix<uint32_t>::ipv4_to_int(ip.c_str());
				u_.ip4.s_addr = htonl(u_.ip4.s_addr);
			}
 		}

		explicit ip_address(uint32_t ip_in_host_byte_order[4]) : family_(AF_INET6), associated_port(0)
		{
			memset(&u_, 0, sizeof(u_));
			u_.ip6.__in6_u.__u6_addr32[0] = htonl(ip_in_host_byte_order[0]);
			u_.ip6.__in6_u.__u6_addr32[1] = htonl(ip_in_host_byte_order[1]);
			u_.ip6.__in6_u.__u6_addr32[2] = htonl(ip_in_host_byte_order[2]);
			u_.ip6.__in6_u.__u6_addr32[3] = htonl(ip_in_host_byte_order[3]);
		}

		ip_address(const ip_address &other) : family_(other.family_), associated_port(other.associated_port)
		{
			::memcpy(&u_, &other.u_, sizeof(other.u_));
		}
		virtual ~ip_address()
		{
		}
		const ip_address &
		operator=(const ip_address &other)
		{
			family_ = other.family_;
			::memcpy(&u_, &other.u_, sizeof(u_));
			associated_port = other.associated_port;

			return *this;
		}
		bool
		operator==(const ip_address &other) const;
		bool
		operator!=(const ip_address &other) const;
		bool
		operator<(const ip_address &other) const;
		bool
		operator>(const ip_address &other) const;
		inline std::ostream &
		operator<<(std::ostream &os)
		{

			if (family_ != AF_UNSPEC)
				return os << to_string();
			return os;
		}
		int
		family() const
		{
			return family_;
		}
		in_addr
		ipv4_address() const;
		in6_addr
		ipv6_address() const;
		size_t
		size() const;
		std::string
		to_string() const;
		std::string
		to_anonymized_string() const;
		ip_address
		normalized() const;
		ip_address
		get_host_order_address6() const;
		uint32_t
		get_host_order_address4() const;
		int
		overhead() const;
		bool
		is_empty() const;
		std::string
		get_prefix() const;
		std::string
		get_query() const;

		const unsigned char *
		get_bytes() const
		{
			return (family_ == AF_INET) ? reinterpret_cast<const unsigned char *>(&u_.ip4.s_addr) : reinterpret_cast<const unsigned char *>(&u_.ip6.__in6_u.__u6_addr8[0]);
		}
		unsigned char *
		get_bytes()
		{
			return (family_ == AF_INET) ? reinterpret_cast<unsigned char *>(&u_.ip4.s_addr) : reinterpret_cast<unsigned char *>(&u_.ip6.__in6_u.__u6_addr8[0]);
		}
		bool
		is_link_local() const
		{
			const unsigned char *bytes_ = get_bytes();
			return ((bytes_[0] == 0xfe) && ((bytes_[1] & 0xc0) == 0x80));
		}

		bool
		is_site_local() const
		{

			const unsigned char *bytes_ = get_bytes();
			return ((bytes_[0] == 0xfe) && ((bytes_[1] & 0xc0) == 0xc0));
		}
		bool
		is_multicast() const
		{

			const unsigned char *bytes_ = get_bytes();
			return (bytes_[0] == 0xff);
		}

		/// Determine whether the address is a global multicast address.
		bool
		is_multicast_global() const
		{

			const unsigned char *bytes_ = get_bytes();
			return ((bytes_[0] == 0xff) && ((bytes_[1] & 0x0f) == 0x0e));
		}

		/// Determine whether the address is a link-local multicast address.
		bool
		is_multicast_link_local() const
		{

			const unsigned char *bytes_ = get_bytes();
			return ((bytes_[0] == 0xff) && ((bytes_[1] & 0x0f) == 0x02));
		}

		bool
		is_multicast_node_local() const
		{

			const unsigned char *bytes_ = get_bytes();
			return ((bytes_[0] == 0xff) && ((bytes_[1] & 0x0f) == 0x01));
		}

		bool
		is_multicast_org_local() const
		{

			const unsigned char *bytes_ = get_bytes();
			return ((bytes_[0] == 0xff) && ((bytes_[1] & 0x0f) == 0x08));
		}

		/// Determine whether the address is a site-local multicast address.
		bool
		is_multicast_site_local() const
		{

			const unsigned char *bytes_ = get_bytes();
			return ((bytes_[0] == 0xff) && ((bytes_[1] & 0x0f) == 0x05));
		}
        ip_address operator =(const std::string &ip)
        {
            if (ip.find_first_of(':', 0) != std::string::npos)
            {
                family_ = AF_INET6;
                uint128_t ip128 = prefix<uint128_t>::ipv6_to_int(ip.c_str());
                memcpy(&u_.ip6.__in6_u.__u6_addr8, &ip128, sizeof(uint128_t));
            }
            else if (ip.find_first_of('.', 0) != std::string::npos)
            {

                family_ = AF_INET;
                u_.ip4.s_addr = prefix<uint32_t>::ipv4_to_int(ip.c_str());
                u_.ip4.s_addr = htonl(u_.ip4.s_addr);
            }
            return *this;
        }   
        ip_address operator =(const uint32_t ip_in_host_byte_order)
        {
            memset(&u_, 0, sizeof(u_));
            family_ = AF_INET;
            u_.ip4.s_addr = htonl(ip_in_host_byte_order);
            return *this;
        } 
        ip_address operator =(const in_addr &ip4)
        {
            memset(&u_, 0, sizeof(u_));
            family_ = AF_INET;
            u_.ip4 = ip4;
            return *this;
        }
        ip_address operator =(const in6_addr &ip6)
        {
            memset(&u_, 0, sizeof(u_));
            family_ = AF_INET6;
            u_.ip6 = ip6;
            return *this;
        }
        ip_address operator =(int value_res)
        {
            family_ = AF_INET;
            u_.ip4.s_addr = htonl(value_res);
            return *this;
        }
		static bool is_valid_ipv4(const std::string &ip)
		{
			struct in_addr addr;
			return inet_pton(AF_INET, ip.c_str(), &addr) == 1;
		}
		static bool is_valid_ipv6(const std::string &ip)
		{
			struct in6_addr addr;
			return inet_pton(AF_INET6, ip.c_str(), &addr) == 1;
		}
		static bool is_valid_ip(const std::string &ip)
		{
			return is_valid_ipv4(ip) || is_valid_ipv6(ip);
		}	

        
	private:
		int family_;
		union
		{
			in_addr ip4;
			in6_addr ip6;
		} u_;
		uint16_t associated_port; // TBU

    }; //end of ip_helper class

}//end of namespace happy_proxy
#endif // __IP_HELPER_H__