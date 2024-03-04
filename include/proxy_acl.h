#ifndef _HAPPY_PROXY_ACE_H_ 
#define _HAPPY_PROXY_ACE_H_

// Happy proxy is a c++17 implementation of HAProxy project
// It uses the same configuration file as HAProxy 
// It allows Provallo engine's predictive analytics to be used for load balancing,error prediction and noise reduction.
// It supports configuration from the latest versions of HAPROXY.
// It supports the latest versions of HTTP, TCP, UDP, and TLS. boringssl, openssl, and mbedtls are supported. 
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>

#include "proxy_util.h"
#include "proxy.h"


namespace happy_proxy { 

//each session has a ACE 
//each resource has a ACE 
//each user has a set of ACEs for each resource and session 


enum ACE_type
{
    ACE_type_session = 0,
    ACE_type_resource = 1,
    ACE_type_user = 2,
    ACE_type_group = 3,
    ACE_type_other = 4
};  
enum ACE_permission
{
    ACE_permission_read = 0,
    ACE_permission_write = 1,
    ACE_permission_execute = 2,
    ACE_permission_other = 3
};      
enum ACE_value
{
    ACE_value_allow = 0,
    ACE_value_deny = 1,
    ACE_value_other = 2
};  
enum ACE_value_mask
{
    ACE_value_mask_allow = 0,
    ACE_value_mask_deny = 1,
    ACE_value_mask_other = 2
};
enum ACE_mask
{
    ACE_mask_allow = 0,
    ACE_mask_deny = 1,
    ACE_mask_other = 2
};  

enum ACE_Permissions
{
    ACE_Permissions_read_allow = 0,
    ACE_Permissions_read_deny = 1,
    ACE_Permissions_write_allow = 2,
    ACE_Permissions_write_deny = 3,
    ACE_Permissions_execute_allow = 4,
    ACE_Permissions_execute_deny = 5,
    ACE_Permissions_other = 6
};


template <typename T> 
class ACE
{
    std::atomic<uint64_t> id;
    T ACE_mask; 
    T ACE_value;
    T ACE_value_mask;
    
    public: 
        virtual ~ACE(); 
        void set_ACE(T ACE_value_mask); 
        void set_ACE_mask(T ACE_mask); 
        void set_ACE_value(T ACE_value); 
        void set_ACE_value_mask(T ACE_value_mask); 
        T get_ACE(); 
        T get_ACE_mask(); 
        T get_ACE_value(); 
        T get_ACE_value_mask(); 
        bool check_ACE(T ACE); 
        bool check_ACE_mask(T ACE_mask); 
        bool check_ACE_value(T ACE_value); 
        bool check_ACE_value_mask(T ACE_value_mask); 
        bool check_ACE(T ACE, T ACE_mask); 
        bool check_ACE_value(T ACE_value, T ACE_value_mask); 
        bool check_ACE(T ACE, T ACE_mask, T ACE_value, T ACE_value_mask); 
       
        bool operator == (const ACE<T>& ACE); 
        bool operator != (const ACE<T>& ACE); 
        bool operator > (const ACE<T>& ACE);
        bool operator < (const ACE<T>& ACE);
        bool operator >= (const ACE<T>& ACE);
        bool operator <= (const ACE<T>& ACE);
        //assignment operator
        ACE<T>& operator = (const ACE<T>& ACE);
        //copy constructor
        ACE(const ACE<T>& ACE);
        //move constructor
        ACE(ACE<T>&& ACE);
            //move assignment operator

        ACE<T>& operator = (ACE<T>&& ACE);
        ACE(T id, T ACE_mask, T ACE_value=0, T ACE_value_mask=0);
        ACE();
        static std::atomic<uint64_t> ace_counter;

};
//ACE for each session
//initialalize the counter  

template <typename T>
std::atomic<uint64_t> ACE<T>::ace_counter(0);   

//template implementation
template <typename T>
ACE<T>::ACE(T id, T ACE_mask, T ACE_value, T ACE_value_mask)
{
    this->id = id;
    this->ACE_mask = ACE_mask;
    this->ACE_value = ACE_value;
    this->ACE_value_mask = ACE_value_mask;
}
template <typename T>
ACE<T>::~ACE()
{
    this->id.store(0);
    this->ACE_mask = 0;
    this->ACE_value = 0;
    this->ACE_value_mask = 0;
}
template <typename T>
void ACE<T>::set_ACE(T a)
{
    this->ACE_value = a;
}
template <typename T>
void ACE<T>::set_ACE_mask(T ACE_mask)
{
    this->ACE_mask = ACE_mask;
}
template <typename T>
void ACE<T>::set_ACE_value(T ACE_value)
{
    this->ACE_value = ACE_value;
}
template <typename T>
void ACE<T>::set_ACE_value_mask(T ACE_value_mask)
{
    this->ACE_value_mask = ACE_value_mask;
}
template <typename T>
T ACE<T>::get_ACE()
{
    return this->ACE_value;
}
template <typename T>

T ACE<T>::get_ACE_mask()
{
    return this->ACE_mask;
}
template <typename T>
T ACE<T>::get_ACE_value()
{
    return this->ACE_value;
}
template <typename T>
T ACE<T>::get_ACE_value_mask()
{
    return this->ACE_value_mask;
}
template <typename T>
bool ACE<T>::check_ACE(T a)
{
    return a ==  this->ACE_value;
}
template <typename T>
bool ACE<T>::check_ACE_mask(T ACE_mask)
{
    return this->ACE_mask == ACE_mask;
}
template <typename T>
bool ACE<T>::check_ACE_value(T ACE_value)
{
    return this->ACE_value == ACE_value;
}
template <typename T>
bool ACE<T>::check_ACE_value_mask(T ACE_value_mask)
{
    return this->ACE_value_mask == ACE_value_mask;
}
template <typename T>
bool ACE<T>::check_ACE(T ACE_value, T ACE_mask)
{   

    return  this->ACE_mask == ACE_mask && this->ACE_value == ACE_value;
}
template <typename T>
bool ACE<T>::check_ACE_value(T ACE_value, T ACE_value_mask)
{
    return this->ACE_value == ACE_value && this->ACE_value_mask == ACE_value_mask;
}
template <typename T>
bool ACE<T>::check_ACE(T ACE, T ACE_mask, T ACE_value, T ACE_value_mask)
{
    return this->ACE_mask == ACE_mask && this->ACE_value == ACE_value && this->ACE_value_mask == ACE_value_mask;
}
 template <typename T>
bool ACE<T>::operator == (const ACE<T>& ACE)
{
    return  this->ACE_mask == ACE.ACE_mask && this->ACE_value == ACE.ACE_value && this->ACE_value_mask == ACE.ACE_value_mask;
}
template <typename T>
bool ACE<T>::operator != (const ACE<T>& ACE)
{
    return  this->ACE_mask != ACE.ACE_mask || this->ACE_value != ACE.ACE_value || this->ACE_value_mask != ACE.ACE_value_mask;
}
template <typename T>
bool ACE<T>::operator > (const ACE<T>& ACE)
{
    return   this->ACE_mask > ACE.ACE_mask && this->ACE_value > ACE.ACE_value && this->ACE_value_mask > ACE.ACE_value_mask;
}
template <typename T>
bool ACE<T>::operator < (const ACE<T>& ACE)
{
    return this->ACE_mask < ACE.ACE_mask && this->ACE_value < ACE.ACE_value && this->ACE_value_mask < ACE.ACE_value_mask;
}
template <typename T>
bool ACE<T>::operator >= (const ACE<T>& ACE)
{
    return  this->ACE_mask >= ACE.ACE_mask && this->ACE_value >= ACE.ACE_value && this->ACE_value_mask >= ACE.ACE_value_mask;
}
template <typename T>
bool ACE<T>::operator <= (const ACE<T>& ACE)
{
    return   this->ACE_mask <= ACE.ACE_mask && this->ACE_value <= ACE.ACE_value && this->ACE_value_mask <= ACE.ACE_value_mask;
}
//assignment operator
template <typename T>
ACE<T>& ACE<T>::operator = (const ACE<T>& ACE)
{
    //dont copy the id
    this->ACE_mask = ACE.ACE_mask;
    this->ACE_value = ACE.ACE_value;
    this->ACE_value_mask = ACE.ACE_value_mask;
    return *this;
}   
//copy constructor
template <typename T>   
ACE<T>::ACE(const ACE<T>& ace) : id(0)
{

    ACE::ace_counter++;
    this->id.store(ACE::ace_counter);
    this->ACE_mask = ace.ACE_mask;
    this->ACE_value = ace.ACE_value;
    this->ACE_value_mask = ace.ACE_value_mask;
}   

//move constructor
template <typename T>   
ACE<T>::ACE(ACE<T>&& ace )
{   
    //move the id
    this->id.store(ace.id.load() );
    
    this->ACE_mask = ace.ACE_mask;
    this->ACE_value = ace.ACE_value;
    this->ACE_value_mask = ace.ACE_value_mask;
}   
//copy assignment operator  
template <typename T>   
ACE<T>& ACE<T>::operator = (ACE<T>&& ace)
{
    //move the id
    this->id.store(ace.id);
     
    this->ACE_mask = ace.ACE_mask;
    this->ACE_value = ace.ACE_value;
    this->ACE_value_mask = ace.ACE_value_mask;
    return *this;
} 

//default ACE constructor
template <typename T>
ACE<T>::ACE()
{
    ACE::ace_counter++;
    this->id.store(ACE::ace_counter);
    this->ACE_mask = 0;
    this->ACE_value = 0;
    this->ACE_value_mask = 0;   
}

 class SessionACE 
 {
    ACE<int64_t> session_ACE; //session involves a session id, a session ACE mask, and a session ACE value
    int64_t resource_id;
    int64_t user_id;
    
    public:

    //constructor
    //generate new session ACE 
    SessionACE();
    bool operator == (const ACE<int64_t>& session_ACE); 
    bool operator != (const ACE<int64_t>& session_ACE);
    bool operator > (const ACE<int64_t>& session_ACE);
    bool operator < (const ACE<int64_t>& session_ACE);
    bool operator >= (const ACE<int64_t>& session_ACE);
    bool operator <= (const ACE<int64_t>& session_ACE);
    //assignment operator
    ACE<int64_t>& operator = (const ACE<int64_t>& session_ACE);
    //copy constructor
    SessionACE(const ACE<int64_t>& session_ACE);
    //move constructor
    SessionACE(ACE<int64_t>&& session_ACE);
    //copy assignment operator
    ACE<int64_t>& operator = (ACE<int64_t>&& session_ACE);
    //move assignment operator
   

      
 }; 
 
 //acl for each user
 class ResourceACL 
 {
    int64_t resource_id;
    int16_t resource_type;
    ACE_Permissions _ACE_permission;
    std::vector<ACE<int64_t>> resource_ACEs; //resource involves a resource id, a resource ACE mask, and a resource ACE value 
    std::vector<int64_t> user_ids; //user ids that have access to this resource 
    
    public: 
    //constructor
    //generate new resource ACE
    ResourceACL();
    bool operator == (const ACE<int64_t>& resource_ACE);
    bool operator != (const ACE<int64_t>& resource_ACE);
    bool operator > (const ACE<int64_t>& resource_ACE);
    bool operator < (const ACE<int64_t>& resource_ACE);
    bool operator >= (const ACE<int64_t>& resource_ACE);
    bool operator <= (const ACE<int64_t>& resource_ACE);
    //assignment operator
    ACE<int64_t>& operator = (const ACE<int64_t>& resource_ACE);
    //copy constructor
    ResourceACL(const ACE<int64_t>& resource_ACE);
    //move constructor

    ResourceACL(ACE<int64_t>&& resource_ACE);
    //copy assignment operator
    ACE<int64_t>& operator = (ACE<int64_t>&& resource_ACE);
    //move assignment operator

 
    //copy constructor          


 };
 class ACLGroup 
    {
      int64_t group_id;
        std::vector<int64_t> user_ids; //user ids that belong to this group
        std::vector<int64_t> resource_ids; //resource ids that this group has access to
        std::vector<ACE<int64_t>> group_ACEs; //group involves a group id, a group ACE mask, and a group ACE value
        std::vector<ACE_Permissions> group_Permissions; //group involves a group id, a group ACE mask, and a group ACE value 

        public:
        //constructor
        //generate new group ACE
        ACLGroup();
        bool operator == (const ACE<int64_t>& group_ACE);
        bool operator != (const ACE<int64_t>& group_ACE);
        bool operator > (const ACE<int64_t>& group_ACE);

        bool operator < (const ACE<int64_t>& group_ACE);
        bool operator >= (const ACE<int64_t>& group_ACE);
        bool operator <= (const ACE<int64_t>& group_ACE);
        //assignment operator
        ACE<int64_t>& operator = (const ACE<int64_t>& group_ACE);
        //copy constructor
        ACLGroup(const ACE<int64_t>& group_ACE);
        //move constructor
        ACLGroup(ACE<int64_t>&& group_ACE);
        //copy assignment operator
        ACE<int64_t>& operator = (ACE<int64_t>&& group_ACE);
        //move assignment operator
        //ACE<int64_t>& operator = (const ACE<int64_t>& group_ACE);
        
        
        void add_user_id(int64_t user_id);
        void add_resource_id(int64_t resource_id);
        void add_group_Permisssions(ACE_Permissions ACE_Permissions);
        void add_group_ACE(ACE<int64_t>& group_ACE);

        std::vector<int64_t> get_user_ids()const;
        std::vector<int64_t> get_resource_ids()const;
        std::vector<ACE_Permissions> get_group_Permissions()const;
        std::vector<ACE<int64_t>> get_group_ACEs()const;



        void add_user_ids(std::vector<int64_t>& user_ids);
  };
 class UserACL 
 {
    int64_t user_id;

    std::vector<int64_t> group_ids; //group involves a group id, a group ACE mask, and a group ACE value     
    //
    std::vector<ACE<int64_t>> user_ACEs; //user involves a user id, a user ACE mask, and a user ACE value

    std::vector<int64_t> resource_ids; //resource ids that this user has access to 
    
    public: 
    //constructor
    //generate new user ACE
    UserACL();
    bool operator == (const ACE<int64_t>& user_ACE);
    bool operator != (const ACE<int64_t>& user_ACE);
    bool operator > (const ACE<int64_t>& user_ACE);
    bool operator < (const ACE<int64_t>& user_ACE);
    bool operator >= (const ACE<int64_t>& user_ACE);
    bool operator <= (const ACE<int64_t>& user_ACE);
    //assignment operator
    ACE<int64_t>& operator = (const ACE<int64_t>& user_ACE);
    //copy constructor
    UserACL(const ACE<int64_t>& user_ACE);
    //move constructor
    UserACL(ACE<int64_t>&& user_ACE);
    //copy assignment operator
    ACE<int64_t>& operator = (ACE<int64_t>&& user_ACE);
    //move assignment operator
   // ACE<int64_t>& operator = (const ACE<int64_t>& user_ACE);
    //copy constructor
    void add_group_ACE(ACE<int64_t>& group_ACE);
    void add_group_id(int64_t group_id);
    void add_user_ACE(ACE<int64_t>& user_ACE);
    void add_resource_id(int64_t resource_id);
    void add_group_ACEs(std::vector<ACE<int64_t>>& group_ACEs);
    void add_group_ids(std::vector<int64_t>& group_ids);
    void add_user_ACEs(std::vector<ACE<int64_t>>& user_ACEs);
    void add_resource_ids(std::vector<int64_t>& resource_ids);
    void remove_group_ACE(ACE<int64_t>& group_ACE);
    void remove_group_id(int64_t group_id);
    void remove_user_ACE(ACE<int64_t>& user_ACE);
    void remove_resource_id(int64_t resource_id);
    void remove_group_ACEs(std::vector<ACE<int64_t>>& group_ACEs);
    void remove_group_ids(std::vector<int64_t>& group_ids);

 
 };

    class SessionACL 
    {
        SessionACE session_ACE; 
        std::vector<int64_t> resource_ids; //resource ids that this session has access to 
        std::vector<int64_t> user_ids; //user ids that this session has access to 
        public:
        //constructor
        //generate new session ACE
        SessionACL();
        bool operator == (const ACE<int64_t>& session_ACE);
        bool operator != (const ACE<int64_t>& session_ACE);

        bool operator > (const ACE<int64_t>& session_ACE);
        bool operator < (const ACE<int64_t>& session_ACE);
        bool operator >= (const ACE<int64_t>& session_ACE);
        bool operator <= (const ACE<int64_t>& session_ACE);
        //assignment operator
        ACE<int64_t>& operator = (const ACE<int64_t>& session_ACE);
        //copy constructor
        SessionACL(const ACE<int64_t>& session_ACE);
        //move constructor
        SessionACL(ACE<int64_t>&& session_ACE);
        //copy assignment operator
        ACE<int64_t>& operator = (ACE<int64_t>&& session_ACE);
        //move assignment operator
       // ACE<int64_t>& operator = (const ACE<int64_t>& session_ACE);
        //copy constructor
        void add_resource_id(int64_t resource_id);
        void add_user_id(int64_t user_id);
        void add_resource_ids(std::vector<int64_t>& resource_ids);
        void add_user_ids(std::vector<int64_t>& user_ids);
        void remove_resource_id(int64_t resource_id);
        void remove_user_id(int64_t user_id);
        void remove_resource_ids(std::vector<int64_t>& resource_ids);

        void remove_user_ids(std::vector<int64_t>& user_ids);

    };
    //acl/ace/user/group/session manager
    class AAUGSManager
    {
        
        std::vector<SessionACL> session_ACLs;
        std::vector<ResourceACL> resource_ACLs;
        std::vector<UserACL> user_ACLs;
        std::vector<ACLGroup> group_ACLs;
        std::vector<SessionACE> session_ACEs;
        std::vector<ACE<int64_t>> resource_ACEs;
        std::vector<ACE<int64_t>> user_ACEs;
        std::vector<ACE<int64_t>> group_ACEs;
       
        std::vector<int64_t> resource_ids;
        std::vector<int64_t> users;
        std::vector<int64_t> groups;
        std::vector<int64_t> active_sessions;
        std::vector<int64_t> inactive_sessions;
        std::vector<int64_t> active_resources;
        std::vector<int64_t> inactive_resources;
        
        std::vector<int64_t> revoked_users;
        std::vector<int64_t> revoked_sessions;
        std::vector<ACE_Permissions> group_Permissions;
        std::vector<ACE_Permissions> user_Permissions;
        //construct map with indexes : 
        //std::map<resource_id, resource_ACLs_index>
        //std::map<user_id, user_ACLs_index> 
        //std::map<group_id, group_ACLs_index>
        //std::map<session_id, session_ACLs_index>
        std::map<int64_t, int64_t> resource_ACLs_index_map; 
        std::map<int64_t, int64_t> user_ACLs_index_map; 
        std::map<int64_t, int64_t> group_ACLs_index_map; 
        std::map<int64_t, int64_t> session_ACLs_index_map; 
        std::map<int64_t, int64_t> resource_ACEs_index_map;
        std::map<int64_t, int64_t> user_ACEs_index_map;
        std::map<int64_t, int64_t> group_ACEs_index_map;
        std::map<int64_t, int64_t> session_ACEs_index_map;
        std::map<int64_t, int64_t> resource_ids_index_map;
        std::map<int64_t, int64_t> user_ids_index_map;
        std::map<int64_t, int64_t> group_ids_index_map;
        std::map<int64_t, int64_t> session_ids_index_map;
        std::map<int64_t, int64_t> group_Permissions_index_map;
        std::map<int64_t, int64_t> user_Permissions_index_map;
        
        public:
        //constructor
        AAUGSManager();
        //destructor
        ~AAUGSManager();
        //getters : 
        //get resource ACLs
        std::vector<ResourceACL> get_resource_ACLs(int64_t user_id)const;
        //get user ACLs
        std::vector<UserACL> get_user_ACLs(int64_t user_id)const;
        //get group ACLs
        std::vector<ACLGroup> get_group_ACLs(int64_t user_id)const;
        //get session ACLs
        std::vector<SessionACL> get_session_ACLs(int64_t user_id)const;
        
        SessionACL* get_session_ACL(int64_t session_id);
        ResourceACL* get_resource_ACL(int64_t resource_id);
        UserACL* get_user_ACL(int64_t user_id);
        ACLGroup* get_group_ACL(int64_t group_id);
        SessionACE* get_session_ACE(int64_t session_id);
        ACE<int64_t>* get_resource_ACE(int64_t resource_id);
        ACE<int64_t>* get_user_ACE(int64_t user_id);
        ACE<int64_t>* get_group_ACE(int64_t group_id);
        std::vector<ACE_Permissions*> get_group_Permissions(int64_t group_id);        
        std::vector<ACE_Permissions*> get_user_Permissions(int64_t user_id); 
        std::vector<int64_t> get_resource_ids()const;
        ACE_Permissions get_group_Permissions(int64_t group_id,int64_t resource_id);
        ACE_Permissions get_user_Permissions(int64_t user_id,int64_t resource_id);
        //get all resource ACLs 
        std::vector<ResourceACL> get_resource_ACLs()const;
        //get all the users ACLs
        std::vector<UserACL> get_user_ACLs()const;
        //get group ACLs
        std::vector<ACLGroup> get_group_ACLs()const;
        //get session ACLs
        std::vector<SessionACL> get_session_ACLs()const;
        //setters :
         
        //add resource ACL
        void add_resource_ACL(ResourceACL& resource_ACL);
        //add user ACL
        void add_user_ACL(UserACL& user_ACL);
        //add group ACL
        void add_group_ACL(ACLGroup& group_ACL);
        //add session ACL
        void add_session_ACL(SessionACL& session_ACL);
        //add resource ACE
        void add_resource_ACE(ACE<int64_t>& resource_ACE);
        //add user ACE for session ACL on resource ACL:
        void add_user_ACE(ACE<int64_t>& user_ACE); 
        //add group ACE for session ACL on resource ACL:
        void add_group_ACE(ACE<int64_t>& group_ACE);
        //add session ACE for session ACL on resource ACL:
        void add_session_ACE(ACE<int64_t>& session_ACE);
        //add resource id
        void add_resource_id(int64_t resource_id);
        //add user id
        void add_user_id(int64_t user_id);
        //add group id
        void add_group_id(int64_t group_id);
        //add session id
        void add_session_id(int64_t session_id);
        //add group Permissions
        void add_group_Permissions(uint64_t group_id,ACE_Permissions group_Permissions);
        //add user Permissions
        void add_user_Permissions(uint64_t user_id ,ACE_Permissions user_Permissions);
        //add resource ACLs
        void add_resource_ACLs(std::vector<ResourceACL>& resource_ACLs);
        //add user ACLs
        void add_user_ACLs(std::vector<UserACL>& user_ACLs);
        //destroy resource ACL
        void destroy_resource_ACL(int64_t resource_id);
        //destroy user ACL
        void destroy_user_ACL(int64_t user_id);
        //destroy group ACL
        void destroy_group_ACL(int64_t group_id);
        //destroy session ACL
        void destroy_session_ACL(int64_t session_id);
        //destroy resource ACLs
        void destroy_resource_ACLs(std::vector<int64_t>& resource_ids);

        //unmap:
        //unmap resource ACL
        void unmap_resource_ACL(int64_t resource_id);
        //unmap user ACL
        void unmap_user_ACL(int64_t user_id , int64_t resource_id);
        //unmap group ACL
        void unmap_group_ACL(int64_t group_id , int64_t resource_id);
        //unmap session ACL
        void unmap_session_ACL(int64_t session_id,int64_t resource_id,int64_t user_id); 
        //unmap resource ACLs
        void unmap_resource_ACLs(std::vector<int64_t>& resource_ids);
        //unmap user ACLs
     };

     template <typename T>
     class NetworkACE : public ACE<T> 
     {

        std::atomic<uint64_t> network_id;
        T network_ACE_mask;
        T network_ACE_value;
        T network_ACE_value_mask;
        public:
        //constructor
        //default empty constructor
        NetworkACE();
        ~NetworkACE();
        void set_network_ACE(T network_ACE);
        void set_network_ACE_mask(T network_ACE_mask);
        void set_network_ACE_value(T network_ACE_value);
        void set_network_ACE_value_mask(T network_ACE_value_mask);
        T get_network_ACE();
        T get_network_ACE_mask();
        T get_network_ACE_value();
        T get_network_ACE_value_mask();
        T operator () (T network_ACE, T network_ACE_mask, T network_ACE_value, T network_ACE_value_mask); 
        //conversion operator
        
         bool operator == (const NetworkACE<T>& network_ACE); 
        bool operator != (const NetworkACE<T>& network_ACE);
        bool operator > (const NetworkACE<T>& network_ACE);
        bool operator < (const NetworkACE<T>& network_ACE);
        bool operator >= (const NetworkACE<T>& network_ACE);
        bool operator <= (const NetworkACE<T>& network_ACE);
        //assignment operator
        NetworkACE<T>& operator = (const NetworkACE<T>& network_ACE);
        //copy assignment operator
        NetworkACE<T>& operator = (NetworkACE<T>&& network_ACE);
         //constructors  
        NetworkACE(T network_id, T network_ACE_mask, T network_ACE_value, T network_ACE_value_mask);
        NetworkACE(const NetworkACE<T>& network_ACE);
        NetworkACE(NetworkACE<T>&& network_ACE);

        static std::atomic<uint64_t> net_counter;
     };

        template <typename T>
        std::atomic<uint64_t> NetworkACE<T>::net_counter(0);   

        //template implementation
        template
        <typename T>
        NetworkACE<T>::NetworkACE()
        {
            this->network_id.store(NetworkACE::net_counter++ );
            
        }
        template <typename T>   
        NetworkACE<T>::NetworkACE(T net_id, T network_ACE_mask, T network_ACE_value, T network_ACE_value_mask)
        {
            this->network_id.store(net_id);
            this->network_ACE_mask = network_ACE_mask;
            this->network_ACE_value = network_ACE_value;
            this->network_ACE_value_mask = network_ACE_value_mask;
        }   
        template <typename T>
        NetworkACE<T>::~NetworkACE()
        {
             this->network_ACE_mask = 0;
            this->network_ACE_value = 0;
            this->network_ACE_value_mask = 0;
        }   
        template <typename T>   
        void NetworkACE<T>::set_network_ACE(T network_ACE)
        {
            this->network_ACE = network_ACE;
        }   
        template <typename T>   
        void NetworkACE<T>::set_network_ACE_mask(T network_ACE_mask)
        {
            this->network_ACE_mask = network_ACE_mask;
        }   
        template <typename T>   
        void NetworkACE<T>::set_network_ACE_value(T network_ACE_value)
        {
            this->network_ACE_value = network_ACE_value;
        }
        template <typename T>
        void NetworkACE<T>::set_network_ACE_value_mask(T network_ACE_value_mask)
        {
            this->network_ACE_value_mask = network_ACE_value_mask;
        }
        template <typename T>

        T NetworkACE<T>::get_network_ACE()
        {
            return this->network_ACE;
        }
        template <typename T>
        T NetworkACE<T>::get_network_ACE_mask()
        {
            return this->network_ACE_mask;
        }
        template <typename T>
        T NetworkACE<T>::get_network_ACE_value()
        {
            return this->network_ACE_value;
        }
        template <typename T>
        T NetworkACE<T>::get_network_ACE_value_mask()
        {
            return this->network_ACE_value_mask;
        }
        template <typename T>
        T NetworkACE<T>::operator () (T network_ACE, T network_ACE_mask, T network_ACE_value, T network_ACE_value_mask)
        {
            this->network_ACE = network_ACE;
            this->network_ACE_mask = network_ACE_mask;
            this->network_ACE_value = network_ACE_value;
            this->network_ACE_value_mask = network_ACE_value_mask;
            return this->network_ACE;
        }
        /*template <typename T>
        T NetworkACE<T>::operator ()const
        {
            return this->network_ACE;
        }
        */
        template <typename T>
        bool NetworkACE<T>::operator == (const NetworkACE<T>& network_ACE)
        {
            return this->network_ACE == network_ACE.network_ACE && this->network_ACE_mask == network_ACE.network_ACE_mask && this->network_ACE_value == network_ACE.network_ACE_value && this->network_ACE_value_mask == network_ACE.network_ACE_value_mask;
        }
        template <typename T>
        bool NetworkACE<T>::operator != (const NetworkACE<T>& network_ACE)
        {
            return this->network_ACE != network_ACE.network_ACE || this->network_ACE_mask != network_ACE.network_ACE_mask || this->network_ACE_value != network_ACE.network_ACE_value || this->network_ACE_value_mask != network_ACE.network_ACE_value_mask;
        }
        template <typename T>
        bool NetworkACE<T>::operator > (const NetworkACE<T>& network_ACE)
        {
            return this->network_ACE > network_ACE.network_ACE && this->network_ACE_mask > network_ACE.network_ACE_mask && this->network_ACE_value > network_ACE.network_ACE_value && this->network_ACE_value_mask > network_ACE.network_ACE_value_mask;
        }
        template <typename T>
        bool NetworkACE<T>::operator < (const NetworkACE<T>& network_ACE)
        {
            return this->network_ACE < network_ACE.network_ACE && this->network_ACE_mask < network_ACE.network_ACE_mask && this->network_ACE_value < network_ACE.network_ACE_value && this->network_ACE_value_mask < network_ACE.network_ACE_value_mask;
        }
        template <typename T>
        bool NetworkACE<T>::operator >= (const NetworkACE<T>& network_ACE)
        {
            return this->network_ACE >= network_ACE.network_ACE && this->network_ACE_mask >= network_ACE.network_ACE_mask && this->network_ACE_value >= network_ACE.network_ACE_value && this->network_ACE_value_mask >= network_ACE.network_ACE_value_mask;
        }
        template <typename T>
        bool NetworkACE<T>::operator <= (const NetworkACE<T>& network_ACE)
        {
            return this->network_ACE <= network_ACE.network_ACE && this->network_ACE_mask <= network_ACE.network_ACE_mask && this->network_ACE_value <= network_ACE.network_ACE_value && this->network_ACE_value_mask <= network_ACE.network_ACE_value_mask;
        }
        //assignment operator
        template <typename T>
        NetworkACE<T>& NetworkACE<T>::operator = (const NetworkACE<T>& network_ACE)
        {
            this->network_ACE = network_ACE.network_ACE;
            this->network_ACE_mask = network_ACE.network_ACE_mask;
            this->network_ACE_value = network_ACE.network_ACE_value;
            this->network_ACE_value_mask = network_ACE.network_ACE_value_mask;
            return *this;
        }
        //copy constructor
        template <typename T>
        NetworkACE<T>::NetworkACE(const NetworkACE<T>& network_ACE)
        {
            this->network_ACE = network_ACE.network_ACE;
            this->network_ACE_mask = network_ACE.network_ACE_mask;
            this->network_ACE_value = network_ACE.network_ACE_value;
            this->network_ACE_value_mask = network_ACE.network_ACE_value_mask;
        }
        //move constructor
        template <typename T>
        NetworkACE<T>::NetworkACE(NetworkACE<T>&& network_ACE)
        {
            this->network_ACE = network_ACE.network_ACE;
            this->network_ACE_mask = network_ACE.network_ACE_mask;
            this->network_ACE_value = network_ACE.network_ACE_value;
            this->network_ACE_value_mask = network_ACE.network_ACE_value_mask;
        }
        //copy assignment operator
        template <typename T>
        NetworkACE<T>& NetworkACE<T>::operator = (NetworkACE<T>&& network_ACE)
        {
            this->network_ACE = network_ACE.network_ACE;
            this->network_ACE_mask = network_ACE.network_ACE_mask;
            this->network_ACE_value = network_ACE.network_ACE_value;
            this->network_ACE_value_mask = network_ACE.network_ACE_value_mask;
            return *this;
        }
        


} // namespace happy_proxy

#endif