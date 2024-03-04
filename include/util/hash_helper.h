#ifndef __HASH_HELPER_H_
#define __HASH_HELPER_H_


#include <stdint.h>
#include <string>
#include <vector>

//c++ hash helper
#include <functional>

namespace happy_proxy
{
    std::uint64_t wang64a(const std::vector<uint8_t> &data);
    template <typename T> 
    class hash_helper
    {
        protected:
        std::function<T (const std::string&)> string_hash_function;  
        std::function<T (const std::vector<uint8_t>&)> bytes_hash_function; 
    public:
        hash_helper()
        {
            string_hash_function = std::hash<std::string>();
            bytes_hash_function =  wang64a;
        }
        hash_helper(const hash_helper& other)
        {
            string_hash_function = other.string_hash_function;
            bytes_hash_function = other.bytes_hash_function;
        }   
        hash_helper& operator=(const hash_helper& other)
        {
            if (this != &other)
            {
                string_hash_function = other.string_hash_function;
                bytes_hash_function = other.bytes_hash_function;
            }
            return *this;
        }   
        hash_helper(hash_helper&& other)
        {
            string_hash_function = std::move(other.string_hash_function);
            bytes_hash_function = std::move(other.bytes_hash_function);
        }       
        hash_helper& operator=(hash_helper&& other)
        {
            if (this != &other)
            {
                string_hash_function = std::move(other.string_hash_function);
                bytes_hash_function = std::move(other.bytes_hash_function);
            }
            return *this;
        }   
        void set_string_hash_function(std::function<T (const std::string&)> hash_function)
        {
            string_hash_function = hash_function;
        } 
        void set_bytes_hash_function(std::function<T (const std::vector<uint8_t>&)> hash_function)
        {
            bytes_hash_function = hash_function;
        }
        T hash_string(const std::string& data)
        {
            return string_hash_function(data);
        }
        T hash_bytes(const std::vector<uint8_t>& data)
        {
            return bytes_hash_function(data);
        }
    }; 

    //replace std::hash with a custom hash function:
    
  	
	inline uint64_t
	fnv1a (const std::string &text)
	{
	 constexpr const  uint64_t fnv_prime = 16777619;
  	 constexpr const uint64_t fnv_offset_basis = 2166136261;

	  uint64_t hash = fnv_offset_basis;
	  for (size_t i = 0; i < text.size (); i++)
	    {
	      hash ^= text[i];
	      hash *= fnv_prime;
	    }
	  return hash;
	}	


	  inline uint64_t wang64(const uint64_t &x) {
  		std::uint64_t y(x);
		y = (~y) + (y << 21);  // y = (y << 21) - y - 1;
		y = y ^ (y >> 24);
		y = (y + (y << 3)) + (y << 8);  // y * 265
		y = y ^ (y >> 14);
		y = (y + (y << 2)) + (y << 4);  // y * 21
		y = y ^ (y >> 28);
		y = y + (y << 31);
		return y;
	}

    //wang64 for vector<uint8_t>
    inline uint64_t wang64a(const std::vector<uint8_t> &data) {
        std::uint64_t y(0);
        for (size_t i = 0; i < data.size(); i++)
        {
            y = wang64(y + data[i]);
        }
        return y;
    }   

	inline  uint32_t wang32(const uint32_t &x) {
  		uint32_t y(x);
		y = (~y) + (y << 15);  // y = (y << 15) - y - 1;
		y = y ^ (y >> 12);
		y = (y + (y << 2)) + (y << 4);  // y * 21
		y = y ^ (y >> 9);
		y = (y + (y << 3)) + (y << 4);  // y * 28
		y = y ^ (y >> 23);
		y = y + (y << 1) + (y << 4);  // y * 21 * 5
		return y;
	}
	inline uint16_t wang16(const uint16_t &x) {
  		uint16_t y(x);
		y = (~y) + (y << 7);  // y = (y << 7) - y - 1;
		y = y ^ (y >> 4);
		y = (y + (y << 3)) + (y << 4);  // y * 21
		y = y ^ (y >> 10);
		y = y + (y << 1) + (y << 4);  // y * 21 * 5
		return y;
	}
    inline  uint8_t wang8(const uint8_t &x) {
              		uint8_t y(x);

                            y = (~y) + (y << 3);  // y = (y <<  
                            y = y ^ (y >> 5);
                            y = (y + (y << 2)) + (y << 4);  // y * 21
                            y = y ^ (y >> 7);
                            y = y + (y << 1) + (y << 4);  // y * 21 * 5
                            return y;
        }


    class server_id_hash : public hash_helper<uint64_t> 
    {
        std::string name;
        std::string ip_address;
        uint16_t port;

    public:
        server_id_hash(const std::string& name_,const std::string& ip_address_,const uint16_t port):hash_helper(), name(name_),ip_address(ip_address_),port(port)
        {
            //set the hash functions - string and bytes
            set_string_hash_function(fnv1a);
            set_bytes_hash_function(wang64a);
        }

        //hash a server id
        uint64_t hash_server_id()
        {
            std::string server_id = name + ip_address + std::to_string(port);
            return hash_string(server_id);
        }
        std::string get_name() const
        {
            return name;
        }
        std::string get_ip_address() const
        {
            return ip_address;
        }
        uint16_t get_port() const
        {
            return port;
        }

    };  

} /* namespace happy_proxy */

#endif /* __HASH_HELPER_H_ */