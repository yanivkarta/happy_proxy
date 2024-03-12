#ifndef _HAPPY_PROXY_H_
#define _HAPPY_PROXY_H_

#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <mutex>

#include <thread>
#include <atomic>
 

#include "proxy_util.h"
#include "util/exception.h"
#include "util/logger.h"
#include "util/ip_helper.h"

namespace happy_proxy
{


    //proxy server configuration
    enum policy_subject 
    {
        FRONTEND = 0,
        BACKEND = 1,
        LISTEN = 2,
        RESOLVERS = 3,
        PEERS = 4,
        USERLISTS = 5,
        MAILERS = 6,
        CACHES = 7,
        PROGRAMS = 8,
        STATS = 9,
        BLOCKS = 10,
        CUSTOM = 11,
    };

 
    //haproxy compatible balance types 
    enum balance_type
    {
        RANDOM = 0,
        ROUND_ROBIN= 1,
        STATIC_ROUND_ROBIN= 2,
        LEAST_CONNECTIONS = 3,
        SOURCE = 4,
        URI = 5,
        URL_PARAM = 6,
        URL_COOKIE =    7,
        RDP_COOKIE = 8,
        SIP = 9,
        HEADER = 10,
        RATIO =     11,
        QUEUE = 12,
        STICK_TABLE = 80,
        STICKY = 89,
        PROVALLO_PREDICTOR  = 99,
        DYNAMIC_RATIO = 100,
    };


    enum application_acl_type {
        HTTP_REQUEST = 0,
        HTTP_RESPONSE = 1,
        TCP_REQUEST = 2,
        TCP_RESPONSE = 3,
        UDP_REQUEST = 4,
        UDP_RESPONSE = 5,
        OTHER = 6 
    };
    enum appplication_acl_condition{
        DENY_IF = 0,
        ALLOW_IF = 1,
        REDIRECT_IF = 2,
        SET_DST = 3,
        SET_SRC = 4,
        SET_VAR = 5,
        SET_PATH = 6,
        SET_QUERY = 7,
        SET_HEADER = 8,
        SET_COOKIE = 9,
        SET_PATH_PARAM = 10,
        SET_QUERY_PARAM = 11,
        SET_AUTH = 12,
        SET_AUTH_PARAM = 13,
        SET_AUTH_GROUP = 14,
        SET_URI = 15,
        SET_METHOD = 16,
        SET_STATUS = 17
    };
    enum application_acl_action{
        SET = 0,
        DENY = 1,
        ALLOW = 2,
        REDIRECT = 3,
        MODIFY = 4
    };
    //application acls are not parsed, they get vectorized and used 
    //automatically on the fields,headers,cookies,paths,query parameters,auth,auth groups,uri,method,status 
    //they are used to set,deny,allow,redirect, and modify the fields,headers,cookies,paths,query parameters,auth,auth groups,uri,method,status 
    //
    struct application_acl_description
    {
        enum application_acl_type type;
        enum appplication_acl_condition condition;
        enum application_acl_action action;
        std::string value;
        std::string action_value;

    };  
    //server details  

    struct server_details //server details under listen and backend sections 
    {
        std::string name;
        std::string ip_address;
        uint16_t port;
        std::string check;
        std::string check_interval;
        std::string check_down;
        std::string check_rise;
        std::string check_fall;
        std::string check_timeout; 
        std::string check_send;
        std::string check_expect;
        std::string check_http_send;
        std::string check_http_expect;
        std::string check_http_expect_error;
        std::string check_http_expect_nocache; 
        bool operator==(const server_details& rhs) const
        {
            return this->name == rhs.name && this->ip_address == rhs.ip_address && this->port == rhs.port;
        }
        bool operator!=(const server_details& rhs) const
        {
            return this->name != rhs.name || this->ip_address != rhs.ip_address || this->port != rhs.port;
        }
        const server_details& copy(const server_details& rhs)
        {
            this->name = rhs.name;
            this->ip_address = rhs.ip_address;
            this->port = rhs.port;
            this->check = rhs.check;
            this->check_interval = rhs.check_interval;
            this->check_down = rhs.check_down;
            this->check_rise = rhs.check_rise;
            this->check_fall = rhs.check_fall;
            this->check_timeout = rhs.check_timeout;
            this->check_send = rhs.check_send;
            this->check_expect = rhs.check_expect;
            this->check_http_send = rhs.check_http_send;
            this->check_http_expect = rhs.check_http_expect;
            this->check_http_expect_error = rhs.check_http_expect_error;
            this->check_http_expect_nocache = rhs.check_http_expect_nocache;
            return *this;
        } 
        const server_details& operator = (const server_details& rhs)
        {
            return this->copy(rhs);
        }   
        //copy constructor:
        server_details(const server_details& other)
        {
            this->copy(other);
        }
        //move constructor:
        server_details(server_details&& other)
        {
            this->copy(other);
        }
        //default constructor:
        server_details()
        {
            this->name = "";
            this->ip_address = "";
            this->port = 0;
            this->check = "";
            this->check_interval = "";
            this->check_down = "";
            this->check_rise = "";
            this->check_fall = "";
            this->check_timeout = "";
            this->check_send = "";
            this->check_expect = "";
            this->check_http_send = "";
            this->check_http_expect = "";
            this->check_http_expect_error = "";
            this->check_http_expect_nocache = "";
        }   

    };

    //cache entries
    struct cache_entries 
    { 
        //cache entries
        std::string name;
        std::string max_size;
        std::string max_age;
        std::string max_stale;
        std::string min_fresh;
        std::string no_cache;
        std::string no_store;
        std::string no_transform;
        std::string private_cache;
        std::string proxy_cache;
        std::string public_cache;
        std::string must_revalidate;
        std::string proxy_revalidate;
        std::string stale_if_error;
        std::string stale_while_revalidate;
        std::string ignore_cache_control;
        std::string ignore_no_cache;
        std::string ignore_auth;
        std::string ignore_private;
        std::string ignore_no_store; 
               
    };

    //construct configuration trees from HAProxy configuration files
    //parse and apply .cfg files 
        //parse/apply each section : 

        struct global_parameters 
        {
            int32_t maxconnections; 
            int32_t ulimitn;
            int32_t hardstopafter;
            int32_t maxpipesize;
            std::string log;
            std::string log_tag;
            std::string chroot;
            std::string pidfile;
            std::string user;
            std::string group;
            std::string daemon;
            std::string stats_socket;
            std::string stats_timeout;
            std::string stats_bind_process;
            std::string stats_bind_cpu;
            std::string stats_bind_ipc;
            std::string stats_bind_unix;
            std::string stats_socket_level;
            std::string stats_socket_user;
            std::string stats_socket_group;
            std::string nbproc;
            std::string nbthread;
            std::string cpu_map;
            std::string cpu_map_auto;
            std::string cpu_map_base;
            std::string cpu_map_safe;
            std::string cpu_affinity;
            std::string cpu_affinity_list;
            std::string cpu_affinity_auto;
            std::string cpu_affinity_base;
            std::string cpu_affinity_safe;
            std::string thread_affinity;
            std::string thread_affinity_list;
            std::string thread_affinity_auto;
            std::string thread_affinity_base;
            std::string thread_affinity_safe;
            std::string debug;
            std::string quiet;
            std::string verbose;
            std::string daemonize;
            std::string master_worker;
            std::string spread_checks;
            std::string nbproc_balance;
            std::string cpu_threads;
            std::string tune_ssl_default_dh_param;
            std::string ssl_default_bind_ciphers;
            std::string ssl_default_bind_options;
            std::string ssl_default_server_ciphers;
            std::string ssl_default_server_options;
            std::string ssl_default_client_ciphers;
            std::string ssl_default_client_options;
            std::string ssl_default_client_ctx;
            std::string ssl_default_server_ctx;
            std::string ssl_default_verify_depth;
            std::string ssl_default_cache;
            std::string ssl_default_dh_param;
            std::string ssl_default_ecdh_curve;
            std::string ssl_default_options;
            std::string ssl_default_bind_curve;
            std::string ssl_default_bind_ciphersuites;
            std::string ssl_default_bind_ecdh_curve;
            std::string ssl_default_bind_proto;
            std::string ssl_default_server_ciphersuites;
            std::string ssl_default_server_ecdh_curve;
            std::string ssl_default_server_proto;
            std::string ssl_default_client_ciphersuites;
            std::string ssl_default_client_ecdh_curve;
            std::string ssl_default_client_proto;
            //set defauilts in default constructor :

            global_parameters():maxconnections(16384),ulimitn(16384),hardstopafter(0),maxpipesize(1024*1024),log("global"),log_tag(""),chroot(""),
            pidfile("/var/run/happy_proxy.pid"),user("nobody"),group("nobody"),daemon("off"),stats_socket("/var/run/happy_proxy.sock"), 
            stats_timeout("2m"),stats_bind_process("all"),stats_bind_cpu("all"),stats_bind_ipc("all"),stats_bind_unix("all"),stats_socket_level("user"),
            stats_socket_user("nobody"),stats_socket_group("nobody"),nbproc("1"),nbthread("1"),cpu_map(""),cpu_map_auto("false"),cpu_map_base("0"),
            cpu_map_safe("false"),cpu_affinity(""),cpu_affinity_list(""),cpu_affinity_auto("false"),cpu_affinity_base("0"),cpu_affinity_safe("false"),
            thread_affinity(""),thread_affinity_list(""),thread_affinity_auto("false"),thread_affinity_base("0"),thread_affinity_safe("false"),debug("off"),
            quiet("off"),verbose("off"),daemonize("off"),master_worker("off"),spread_checks("off"),nbproc_balance("off"),cpu_threads("off"),
            tune_ssl_default_dh_param("on"),ssl_default_bind_ciphers("DEFAULT"),ssl_default_bind_options("no-sslv3 no-tlsv10 no-tlsv11"),
            ssl_default_server_ciphers("DEFAULT"),ssl_default_server_options("no-sslv3 no-tlsv10 no-tlsv11"),ssl_default_client_ciphers("DEFAULT"),
            ssl_default_client_options("no-sslv3 no-tlsv10 no-tlsv11"),ssl_default_client_ctx(""),ssl_default_server_ctx(""),ssl_default_verify_depth("3"),
            ssl_default_cache("off"),ssl_default_dh_param("/etc/ssl/dhparam.pem"),ssl_default_ecdh_curve("auto"),ssl_default_options("no-sslv3 no-tlsv10 no-tlsv11"),
            ssl_default_bind_curve("auto"),ssl_default_bind_ciphersuites("DEFAULT"),ssl_default_bind_ecdh_curve("auto"),ssl_default_bind_proto("all"),ssl_default_server_ciphersuites("DEFAULT"),ssl_default_server_ecdh_curve("auto"),ssl_default_server_proto("all"),
            ssl_default_client_ciphersuites("DEFAULT"),ssl_default_client_ecdh_curve("auto"),ssl_default_client_proto("all")
            {
            }

            void print()const
            {
                std::cout<<"[+]Global Parameters: \n";
                std::cout<<"[+]maxconnections: "<<maxconnections<<"\n";
                std::cout<<"[+]ulimitn: "<<ulimitn<<"\n";
                std::cout<<"[+]hardstopafter: "<<hardstopafter<<"\n";
                std::cout<<"[+]maxpipesize: "<<maxpipesize<<"\n";
                std::cout<<"[+]log: "<<log<<"\n";
                std::cout<<"[+]log_tag: "<<log_tag<<"\n";
                std::cout<<"[+]chroot: "<<chroot<<"\n";
                std::cout<<"[+]pidfile: "<<pidfile<<"\n";
                std::cout<<"[+]user: "<<user<<"\n";
                std::cout<<"[+]group: "<<group<<"\n";
                std::cout<<"[+]daemon: "<<daemon<<"\n";
                std::cout<<"[+]stats_socket: "<<stats_socket<<"\n";
                std::cout<<"[+]stats_timeout: "<<stats_timeout<<"\n";
                std::cout<<"[+]stats_bind_process: "<<stats_bind_process<<"\n";
                std::cout<<"[+]stats_bind_cpu: "<<stats_bind_cpu<<"\n";

                std::cout<<"[+]stats_bind_ipc: "<<stats_bind_ipc<<"\n";
                std::cout<<"[+]stats_bind_unix: "<<stats_bind_unix<<"\n";
                std::cout<<"[+]stats_socket_level: "<<stats_socket_level<<"\n";
                std::cout<<"[+]stats_socket_user: "<<stats_socket_user<<"\n";
                std::cout<<"[+]stats_socket_group: "<<stats_socket_group<<"\n";
                std::cout<<"[+]nbproc: "<<nbproc<<"\n";
                std::cout<<"[+]nbthread: "<<nbthread<<"\n";
                std::cout<<"[+]cpu_map: "<<cpu_map<<"\n";
                std::cout<<"[+]cpu_map_auto: "<<cpu_map_auto<<"\n";

                std::cout<<"[+]cpu_map_base: "<<cpu_map_base<<"\n";
                std::cout<<"[+]cpu_map_safe: "<<cpu_map_safe<<"\n";
                std::cout<<"[+]cpu_affinity: "<<cpu_affinity<<"\n";

                std::cout<<"[+]cpu_affinity_list: "<<cpu_affinity_list<<"\n";
                std::cout<<"[+]cpu_affinity_auto: "<<cpu_affinity_auto<<"\n";
                std::cout<<"[+]cpu_affinity_base: "<<cpu_affinity_base<<"\n";
                std::cout<<"[+]cpu_affinity_safe: "<<cpu_affinity_safe<<"\n";
                std::cout<<"[+]thread_affinity: "<<thread_affinity<<"\n";
                std::cout<<"[+]thread_affinity_list: "<<thread_affinity_list<<"\n";
                std::cout<<"[+]thread_affinity_auto: "<<thread_affinity_auto<<"\n";
                std::cout<<"[+]thread_affinity_base: "<<thread_affinity_base<<"\n";
                std::cout<<"[+]thread_affinity_safe: "<<thread_affinity_safe<<"\n";
                std::cout<<"[+]debug: "<<debug<<"\n";
                std::cout<<"[+]quiet: "<<quiet<<"\n";
                std::cout<<"[+]verbose: "<<verbose<<"\n";

                std::cout<<"[+]daemonize: "<<daemonize<<"\n";
                std::cout<<"[+]master_worker: "<<master_worker<<"\n";
                std::cout<<"[+]spread_checks: "<<spread_checks<<"\n";
                std::cout<<"[+]nbproc_balance: "<<nbproc_balance<<"\n";
                std::cout<<"[+]cpu_threads: "<<cpu_threads<<"\n";
                std::cout<<"[+]tune_ssl_default_dh_param: "<<tune_ssl_default_dh_param<<"\n";
                std::cout<<"[+]ssl_default_bind_ciphers: "<<ssl_default_bind_ciphers<<"\n";
                std::cout<<"[+]ssl_default_bind_options: "<<ssl_default_bind_options<<"\n";
                std::cout<<"[+]ssl_default_server_ciphers: "<<ssl_default_server_ciphers<<"\n";
                std::cout<<"[+]ssl_default_server_options: "<<ssl_default_server_options<<"\n";
                std::cout<<"[+]ssl_default_client_ciphers: "<<ssl_default_client_ciphers<<"\n";
                std::cout<<"[+]ssl_default_client_options: "<<ssl_default_client_options<<"\n";
                std::cout<<"[+]ssl_default_client_ctx: "<<ssl_default_client_ctx<<"\n";
                std::cout<<"[+]ssl_default_server_ctx: "<<ssl_default_server_ctx<<"\n";
                std::cout<<"[+]ssl_default_verify_depth: "<<ssl_default_verify_depth<<"\n";
                std::cout<<"[+]ssl_default_cache: "<<ssl_default_cache<<"\n";
                std::cout<<"[+]ssl_default_dh_param: "<<ssl_default_dh_param<<"\n";
                std::cout<<"[+]ssl_default_ecdh_curve: "<<ssl_default_ecdh_curve<<"\n";
                std::cout<<"[+]ssl_default_options: "<<ssl_default_options<<"\n";
                std::cout<<"[+]ssl_default_bind_curve: "<<ssl_default_bind_curve<<"\n";
                std::cout<<"[+]ssl_default_bind_ciphersuites: "<<ssl_default_bind_ciphersuites<<"\n";
                std::cout<<"[+]ssl_default_bind_ecdh_curve: "<<ssl_default_bind_ecdh_curve<<"\n";
                std::cout<<"[+]ssl_default_bind_proto: "<<ssl_default_bind_proto<<"\n";
                std::cout<<"[+]ssl_default_server_ciphersuites: "<<ssl_default_server_ciphersuites<<"\n";
                std::cout<<"[+]ssl_default_server_ecdh_curve: "<<ssl_default_server_ecdh_curve<<"\n";
                std::cout<<"[+]ssl_default_server_proto: "<<ssl_default_server_proto<<"\n";

                std::cout<<"[+]ssl_default_client_ciphersuites: "<<ssl_default_client_ciphersuites<<"\n";
                std::cout<<"[+]ssl_default_client_ecdh_curve: "<<ssl_default_client_ecdh_curve<<"\n";
                std::cout<<"[+]ssl_default_client_proto: "<<ssl_default_client_proto<<"\n";
                
            }
            //getters convert strings to global parameter values (i.e. maxconnections, ulimitn, hardstopafter, maxpipesize as integers, rest as strings) :
            int32_t get_maxconnections() const
            {
                return maxconnections;
            }
            int32_t get_ulimitn() const
            {
                 return ulimitn;
            }
            int32_t get_hardstopafter() const
            {
                return hardstopafter;
            }
            int32_t get_maxpipesize() const
            {
                return maxpipesize;
            } 
            std::string get_log() const
            {
                return log;
            }
            std::string get_log_tag() const
            {
                return log_tag;
            }
            std::string get_chroot() const
            {
                return chroot;
            }
            std::string get_pidfile() const
            {
                return pidfile;
            }
            std::string get_user() const
            {
                return user;
            }
            std::string get_group() const
            {
                return group;
            } 


        };
        struct front_end
        {
            std::string name;
            std::string mode;
            std::string bind;
            std::string log;
            int32_t maxconn;
            int32_t maxqueue;
            int32_t backlog;
            std::chrono::seconds timeout_client;
            std::chrono::seconds timeout_server;
            size_t max_retries;
            std::chrono::seconds retry_timeout;
            std::vector<std::string> options;
            std::vector<std::string> acls;
            std::vector<std::string> use_backends;
            //conversion operator to struct server_details :
            //convert frontend to server details
            server_details operator()()const{
                struct server_details server;
                server.name =   name;
                server.ip_address = happy_proxy::Split<std::string>(this->bind, ":")[0];
                server.port = std::stoi(happy_proxy::Split<std::string>(this->bind, ":")[1]);
                server.check = "";
                server.check_interval = "";
                server.check_down = "";
                server.check_rise = "";
                server.check_fall = "";
                server.check_timeout = "";
                server.check_send = "";
                server.check_expect = "";
                server.check_http_send = "";
                server.check_http_expect = "";
                server.check_http_expect_error = "";
                server.check_http_expect_nocache = "";
                return server;  
            }   
        };

        struct back_end
        {
            std::string name;
            std::string mode;
            std::string bind;
            std::string log;
            int32_t maxconn;
            int32_t maxqueue;
            int32_t backlog;
            std::chrono::seconds timeout_client;
            std::chrono::seconds timeout_server;
            size_t max_retries;
            std::chrono::seconds retry_timeout;
            std::vector<std::string> options;
            std::vector<std::string> acls;
            std::vector<std::string> request_rules;
            std::vector<std::string> response_rules;
            std::vector<std::string> servers;
            server_details operator ()()const 
            {
                struct server_details server;
                server.name =   name;
                server.ip_address = happy_proxy::Split<std::string>(this->bind, ":")[0];
                server.port = std::stoi(happy_proxy::Split<std::string>(this->bind, ":")[1]);
                server.check = "";
                server.check_interval = "";
                server.check_down = "";
                server.check_rise = "";
                server.check_fall = "";
                server.check_timeout = "";
                server.check_send = "";
                server.check_expect = "";
                server.check_http_send = "";
                server.check_http_expect = "";
                server.check_http_expect_error = "";
                server.check_http_expect_nocache = "";
                return server;  
            }
        };  
        typedef struct configuration_file{
        
        

        public:
        
        configuration_file(std::map<std::string, std::map<std::string, std::string>> configuration)
        {
             //parse map into config structure 

              //map globals 
            this->_globals.maxconnections = configuration["global"]["maxconn"].length()>1 ? std::stoi(configuration["global"]["maxconn"], nullptr, 10): this->_globals.maxconnections; 

            this->_globals.ulimitn =configuration["global"]["ulimit_n"].length()>1? std::stoi(configuration["global"]["ulimitn"]):  this->_globals.ulimitn;
            this->_globals.hardstopafter = configuration["global"]["hardstopafter"].length()>1?std::stoi(configuration["global"]["hardstopafter"]): this->_globals.hardstopafter;

            this->_globals.maxpipesize = configuration["global"]["maxpipesize"].length()>1? std::stoi(configuration["global"]["maxpipesize"]): this->_globals.maxpipesize;
            this->_globals.log = configuration["global"]["log"].length()>1? configuration["global"]["log"]:this->_globals.log; 

            this->_globals.log_tag = configuration["global"]["log_tag"].length()>1? configuration["global"]["log_tag"]:this->_globals.log_tag; 
            this->_globals.chroot = configuration["global"]["chroot"].length()>1? configuration["global"]["chroot"]:this->_globals.chroot;
            this->_globals.pidfile = configuration["global"]["pidfile"].length()>1? configuration["global"]["pidfile"]:this->_globals.pidfile;
            this->_globals.user = configuration["global"]["uid"].length()>1? configuration["global"]["user"]:this->_globals.user;
            this->_globals.group = configuration["global"]["gid"].length()>1? configuration["global"]["group"]:this->_globals.group; 
            this->_globals.daemon = configuration["global"].find("daemon") != configuration["global"].end() ?1 :  0;
            this->_globals.stats_socket = configuration["global"]["stats_socket"].length()>1? configuration["global"]["stats_socket"]:this->_globals.stats_socket;  
            this->_globals.stats_timeout = configuration["global"]["stats_timeout"].length()>1? configuration["global"]["stats_timeout"]:this->_globals.stats_timeout;
            this->_globals.stats_bind_process = configuration["global"]["stats_bind_process"].length()>1? configuration["global"]["stats_bind_process"]:this->_globals.stats_bind_process;

            this->_globals.stats_bind_cpu = configuration["global"]["stats_bind_cpu"].length()>1? configuration["global"]["stats_bind_cpu"]:this->_globals.stats_bind_cpu;
            this->_globals.stats_bind_ipc = configuration["global"]["stats_bind_ipc"].length()>1? configuration["global"]["stats_bind_ipc"]:this->_globals.stats_bind_ipc;
            this->_globals.stats_bind_unix = configuration["global"]["stats_bind_unix"].length()>1? configuration["global"]["stats_bind_unix"]:this->_globals.stats_bind_unix; 
            this->_globals.stats_socket_level = configuration["global"]["stats_socket_level"].length()>1? configuration["global"]["stats_socket_level"]:this->_globals.stats_socket_level; 
            this->_globals.stats_socket_user = configuration["global"]["stats_socket_user"].length()>1? configuration["global"]["stats_socket_user"]:this->_globals.stats_socket_user;
            this->_globals.stats_socket_group = configuration["global"]["stats_socket_group"].length()>1? configuration["global"]["stats_socket_group"]:this->_globals.stats_socket_group;
            this->_globals.nbproc = configuration["global"]["nbproc"].length()>1? configuration["global"]["nbproc"]:this->_globals.nbproc;
            this->_globals.nbthread = configuration["global"]["nbthread"].length()>1? configuration["global"]["nbthread"]:this->_globals.nbthread; 
            this->_globals.cpu_map = configuration["global"]["cpu_map"].length()>1? configuration["global"]["cpu_map"]:this->_globals.cpu_map;
            this->_globals.cpu_map_auto = configuration["global"]["cpu_map_auto"].length()>1? configuration["global"]["cpu_map_auto"]:this->_globals.cpu_map_auto;
            this->_globals.cpu_map_base = configuration["global"]["cpu_map_base"].length()>1? configuration["global"]["cpu_map_base"]:this->_globals.cpu_map_base;
            this->_globals.cpu_map_safe = configuration["global"]["cpu_map_safe"].length()>1? configuration["global"]["cpu_map_safe"]:this->_globals.cpu_map_safe;
            this->_globals.cpu_affinity = configuration["global"]["cpu_affinity"].length()>1? configuration["global"]["cpu_affinity"]:this->_globals.cpu_affinity;
            this->_globals.cpu_affinity_list = configuration["global"]["cpu_affinity_list"].length()>1? configuration["global"]["cpu_affinity_list"]:this->_globals.cpu_affinity_list;
            this->_globals.cpu_affinity_auto = configuration["global"]["cpu_affinity_auto"].length()>1? configuration["global"]["cpu_affinity_auto"]:this->_globals.cpu_affinity_auto;
            this->_globals.cpu_affinity_base = configuration["global"]["cpu_affinity_base"].length()>1? configuration["global"]["cpu_affinity_base"]:this->_globals.cpu_affinity_base;
            this->_globals.cpu_affinity_safe = configuration["global"]["cpu_affinity_safe"].length()>1? configuration["global"]["cpu_affinity_safe"]:this->_globals.cpu_affinity_safe;

            this->_globals.thread_affinity = configuration["global"]["thread_affinity"].length()>1? configuration["global"]["thread_affinity"]:this->_globals.thread_affinity; 
            this->_globals.thread_affinity_list = configuration["global"]["thread_affinity_list"].length()>1? configuration["global"]["thread_affinity_list"]:this->_globals.thread_affinity_list;
            this->_globals.thread_affinity_auto = configuration["global"]["thread_affinity_auto"].length()>1? configuration["global"]["thread_affinity_auto"]:this->_globals.thread_affinity_auto;
            this->_globals.thread_affinity_base = configuration["global"]["thread_affinity_base"].length()>1? configuration["global"]["thread_affinity_base"]:this->_globals.thread_affinity_base;
            this->_globals.thread_affinity_safe = configuration["global"]["thread_affinity_safe"].length()>1? configuration["global"]["thread_affinity_safe"]:this->_globals.thread_affinity_safe;
            this->_globals.debug = configuration["global"]["debug"].length()>1? configuration["global"]["debug"]:this->_globals.debug;
            this->_globals.quiet = configuration["global"]["quiet"].length()>1? configuration["global"]["quiet"]:this->_globals.quiet;
            this->_globals.verbose = configuration["global"]["verbose"].length()>1? configuration["global"]["verbose"]:this->_globals.verbose;
            this->_globals.daemonize = configuration["global"]["daemonize"].length()>1? configuration["global"]["daemonize"]:this->_globals.daemonize;
            this->_globals.master_worker = configuration["global"]["master_worker"].length()>1? configuration["global"]["master_worker"]:this->_globals.master_worker;
            this->_globals.spread_checks = configuration["global"]["spread_checks"].length()>1? configuration["global"]["spread_checks"]:this->_globals.spread_checks;
            this->_globals.nbproc_balance = configuration["global"]["nbproc_balance"].length()>1? configuration["global"]["nbproc_balance"]:this->_globals.nbproc_balance;
            this->_globals.cpu_threads = configuration["global"]["cpu_threads"].length()>1? configuration["global"]["cpu_threads"]:this->_globals.cpu_threads;
            this->_globals.tune_ssl_default_dh_param = configuration["global"]["tune_ssl_default_dh_param"].length()>1? configuration["global"]["tune_ssl_default_dh_param"]:this->_globals.tune_ssl_default_dh_param;
            this->_globals.ssl_default_bind_ciphers = configuration["global"]["ssl_default_bind_ciphers"].length()>1? configuration["global"]["ssl_default_bind_ciphers"]:this->_globals.ssl_default_bind_ciphers;
            this->_globals.ssl_default_bind_options = configuration["global"]["ssl_default_bind_options"].length()>1? configuration["global"]["ssl_default_bind_options"]:this->_globals.ssl_default_bind_options;
            this->_globals.ssl_default_server_ciphers = configuration["global"]["ssl_default_server_ciphers"].length()>1? configuration["global"]["ssl_default_server_ciphers"]:this->_globals.ssl_default_server_ciphers;
            this->_globals.ssl_default_server_options = configuration["global"]["ssl_default_server_options"].length()>1? configuration["global"]["ssl_default_server_options"]:this->_globals.ssl_default_server_options;
            this->_globals.ssl_default_client_ciphers = configuration["global"]["ssl_default_client_ciphers"].length()>1? configuration["global"]["ssl_default_client_ciphers"]:this->_globals.ssl_default_client_ciphers;

            this->_globals.ssl_default_client_options = configuration["global"]["ssl_default_client_options"].length()>1? configuration["global"]["ssl_default_client_options"]:this->_globals.ssl_default_client_options;
            this->_globals.ssl_default_client_ctx = configuration["global"]["ssl_default_client_ctx"].length()>1? configuration["global"]["ssl_default_client_ctx"]:this->_globals.ssl_default_client_ctx;
            this->_globals.ssl_default_server_ctx = configuration["global"]["ssl_default_server_ctx"].length()>1? configuration["global"]["ssl_default_server_ctx"]:this->_globals.ssl_default_server_ctx; 

            this->_globals.ssl_default_verify_depth = configuration["global"]["ssl_default_verify_depth"].length()>1? configuration["global"]["ssl_default_verify_depth"]:this->_globals.ssl_default_verify_depth;
            this->_globals.ssl_default_cache = configuration["global"]["ssl_default_cache"].length()>1? configuration["global"]["ssl_default_cache"]:this->_globals.ssl_default_cache;
            this->_globals.ssl_default_dh_param = configuration["global"]["ssl_default_dh_param"].length()>1? configuration["global"]["ssl_default_dh_param"]:this->_globals.ssl_default_dh_param;
            this->_globals.ssl_default_ecdh_curve = configuration["global"]["ssl_default_ecdh_curve"].length()>1? configuration["global"]["ssl_default_ecdh_curve"]:this->_globals.ssl_default_ecdh_curve;
            this->_globals.ssl_default_options = configuration["global"]["ssl_default_options"].length()>1? configuration["global"]["ssl_default_options"]:this->_globals.ssl_default_options;
            this->_globals.ssl_default_bind_curve = configuration["global"]["ssl_default_bind_curve"].length()>1? configuration["global"]["ssl_default_bind_curve"]:this->_globals.ssl_default_bind_curve;

            this->_globals.ssl_default_bind_ciphersuites = configuration["global"]["ssl_default_bind_ciphersuites"].length()>1? configuration["global"]["ssl_default_bind_ciphersuites"]:this->_globals.ssl_default_bind_ciphersuites;
            this->_globals.ssl_default_bind_ecdh_curve = configuration["global"]["ssl_default_bind_ecdh_curve"].length()>1? configuration["global"]["ssl_default_bind_ecdh_curve"]:this->_globals.ssl_default_bind_ecdh_curve;
            this->_globals.ssl_default_bind_proto = configuration["global"]["ssl_default_bind_proto"].length()>1? configuration["global"]["ssl_default_bind_proto"]:this->_globals.ssl_default_bind_proto;

            this->_globals.ssl_default_server_ciphersuites = configuration["global"]["ssl_default_server_ciphersuites"].length()>1? configuration["global"]["ssl_default_server_ciphersuites"]:this->_globals.ssl_default_server_ciphersuites;
            this->_globals.ssl_default_server_ecdh_curve = configuration["global"]["ssl_default_server_ecdh_curve"].length()>1? configuration["global"]["ssl_default_server_ecdh_curve"]:this->_globals.ssl_default_server_ecdh_curve;
            this->_globals.ssl_default_server_proto = configuration["global"]["ssl_default_server_proto"].length()>1? configuration["global"]["ssl_default_server_proto"]:this->_globals.ssl_default_server_proto;
            this->_globals.ssl_default_client_ciphersuites = configuration["global"]["ssl_default_client_ciphersuites"].length()>1? configuration["global"]["ssl_default_client_ciphersuites"]:this->_globals.ssl_default_client_ciphersuites;

            this->_globals.ssl_default_client_ecdh_curve = configuration["global"]["ssl_default_client_ecdh_curve"].length()>1? configuration["global"]["ssl_default_client_ecdh_curve"]:this->_globals.ssl_default_client_ecdh_curve;

            this->_globals.ssl_default_client_proto = configuration["global"]["ssl_default_client_proto"].length()>1? configuration["global"]["ssl_default_client_proto"]:this->_globals.ssl_default_client_proto;
            //parse defaults
            
            //parse frontends
            //find all frontends 
            std::map<std::string, std::map<std::string, std::string>> frontends; 
            for(auto& section : configuration)
            {
                if(section.first.find("frontend") != std::string::npos)
                {
                    std::cout<<"[+]Frontend found: "<<section.first<<"\n"; 
                    frontends[section.first] = section.second; 
                    //print the map
                    std::string name = section.second["name"];
                    for(auto& key_value : section.second)
                    {
                        
                        std::cout << "[+] frontend  :" <<name<<","<< key_value.first << " : " << key_value.second << std::endl;
                        //if key_value is name, allocate map entry for frontend 
                        if(key_value.first=="name" && key_value.second.length()>1)
                        {

                            frontends[key_value.second] =  frontends[section.first]; 
                            name = key_value.second;
                        }  
                     }
                }
                else if(section.first.find("listen") != std::string::npos)
                {
                    std::cout<<"[+]Listen found: "<<section.first<<"\n"; 
                    
                    //print the map
                    
                    for (auto& key_value : section.second)
                    {
                        std::cout << "[+] listen  :" << section.first<<","<< key_value.first << " : " << key_value.second << std::endl;
                    }
                    //parse listen section
                    auto & listen_map = section.second; 
                    //add key value pair to listen section 
                    //get from configuration map the listen section


                    //parse the server details
                    //add the value if any from the tokenized string 
                    //parse the entry for server details "server <name> <ip>:<port> check <check> check_interval <check_interval> check_down <check_down> check_rise <check_rise> check_fall <check_fall> check_timeout <check_timeout> check_send <check_send> check_expect <check_expect> check_http_send <check_http_send> check_http_expect <check_http_expect> check_http_expect_error <check_http_expect_error> check_http_expect_nocache <check_http_expect_nocache>" 
                    struct server_details server;
                    //server name,ip and port are under server entry in the listen section
                    auto& entry = listen_map["server"];
                    std::vector<std::string> tokens = happy_proxy::Tokenize(entry, " "); 
                    
                    //parse the tokens
                    if(tokens.size() < 3)
                    {
                        //no listen section found
                        //add default server details[next-hop]
                        server.name = "next-hop";
                        server.ip_address = "0.0.0.0";
                        server.port = 0;
                        server.check = listen_map["check"];
                        server.check_interval = listen_map["check_interval"];
                        server.check_down = listen_map["check_down"];
                        server.check_rise = listen_map["check_rise"];
                        server.check_fall = listen_map["check_fall"];
                        server.check_timeout = listen_map["check_timeout"];

                        server.check_send = listen_map["check_send"];
                         continue;
                    }
                    server.name = tokens[1];
                    auto ip = happy_proxy::Split<std::string>(tokens[2], ":"); 
                    server.ip_address = ip[0];
                    server.port = ip.size()>1? std::stoi( ip[1]):0;
                    server.check = listen_map["check"];
                    server.check_interval = listen_map["check_interval"];
                    server.check_down = listen_map["check_down"];
                    server.check_rise = listen_map["check_rise"];
                    server.check_fall = listen_map["check_fall"];
                    server.check_timeout = listen_map["check_timeout"];
                    server.check_send = listen_map["check_send"];
                    server.check_expect = listen_map["check_expect"];
                    server.check_http_send = listen_map["check_http_send"];
                    server.check_http_expect = listen_map["check_http_expect"];
                    server.check_http_expect_error = listen_map["check_http_expect_error"];

                    server.check_http_expect_nocache = listen_map["check_http_expect_nocache"];
                    this->_servers.push_back(server);
                     
                } //backend:
                else if (section.first.find("backend")!= std::string::npos)
                {
                    std::cout<<"[+]Backend found: "<<section.first<<"\n"; 
                    for (auto& key_value : section.second)
                    {
                        std::cout << "[+] backend  :" << section.first<<","<< key_value.first << " : " << key_value.second << std::endl;
                    }
                    //parse backend section 
                    struct back_end backend;
                    backend.name = section.second["name"];
                    backend.mode = section.second["mode"];
                    backend.bind = section.second["bind"];
                    backend.log = section.second["log"];

                    backend.maxconn = section.second["maxconn"].length()>1? std::stoi(section.second["maxconn"]):0;
                    backend.maxqueue = section.second["maxqueue"].length()>1? std::stoi(section.second["maxqueue"]):0;
                    backend.backlog = section.second["backlog"].length()>1? std::stoi(section.second["backlog"]):0;
                    backend.timeout_client = section.second["timeout client"].length()>1? std::chrono::seconds(std::stoi(section.second["timeout client"])):std::chrono::seconds(0);
                    backend.timeout_server = section.second["timeout server"].length()>1? std::chrono::seconds(std::stoi(section.second["timeout server"])):std::chrono::seconds(0);
                    backend.max_retries = section.second["max_retries"].length()>1? std::stoi(section.second["max_retries"]):0;
                    backend.retry_timeout = section.second["retry_timeout"].length()>1? std::chrono::seconds(std::stoi(section.second["retry_timeout"])):std::chrono::seconds(0);
                    //parse options
                    auto options = happy_proxy::Split<std::string>(section.second["option"], " ");
                    for(auto& option : options)
                    {
                        backend.options.push_back(option);
                    }
                    //parse acls
                    auto acls = happy_proxy::Split<std::string>(section.second["acl"], " ");
                    for(auto& acl : acls)
                    {
                        std::cout<<"[+]Backend ACL found: "<<acl<<"\n";
                        backend.acls.push_back(acl);
                    }   
                    //parse use_backends
                    //auto use_backends = happy_proxy::Split<std::string>(section.second["use_backend"], " ");
                    //for(auto& use_backend : use_backends)
                    //{
                    //    backend.use_backends.push_back(use_backend);
                    //}
                    //parse servers
                    this->_back_ends.push_back(backend);
                    
                }
                else if (section.first.find("resolvers")!= std::string::npos)
                {
                    std::cout<<"[+]Resolvers found: "<<section.first<<"\n"; 
                    for (auto& key_value : section.second)
                    {
                        std::cout << "[+] resolvers  :" << section.first<<","<< key_value.first << " : " << key_value.second << std::endl;
                        resolvers.push_back(section.second[key_value.first]) ;
                    }
                    
                }
                else if (section.first.find("peers")!= std::string::npos)
                {
                    std::cout<<"[+]Peers found: "<<section.first<<"\n"; 
                    for (auto& key_value : section.second)
                    {
                        std::cout << "[+] peers  :" << section.first<<","<< key_value.first << " : " << key_value.second << std::endl;
                        peers.push_back(section.second[key_value.first]) ;
                    }
                }
                else if (section.first.find("userlists")!= std::string::npos)
                {
                    std::cout<<"[+]Userlists found: "<<section.first<<"\n"; 
                    for (auto& key_value : section.second)
                    {
                        std::cout << "[+] userlists  :" << section.first<<","<< key_value.first << " : " << key_value.second << std::endl;
                        userlists.push_back(section.second[key_value.first]) ;
                    }
                }
                else if (section.first.find("mailers")!= std::string::npos)
                {
                    std::cout<<"[+]Mailers found: "<<section.first<<"\n"; 
                    for (auto& key_value : section.second)
                    {

                        std::cout << "[+] mailers  :" << section.first<<","<< key_value.first << " : " << key_value.second << std::endl;
                        mailers.push_back(section.second[key_value.first]) ;
                    }
                }
                else if (section.first.find("caches")!= std::string::npos)
                {
                    std::cout<<"[+]Caches found: "<<section.first<<"\n"; 
                    for (auto& key_value : section.second)
                    {
                        std::cout << "[+] caches  :" << section.first<<","<< key_value.first << " : " << key_value.second << std::endl;
                        //add to cache section
                        this->caches.push_back(section.second[key_value.first]) ; 


                    }
                }
                else if (section.first.find("programs")!= std::string::npos)
                {
                    std::cout<<"[+]Programs found: "<<section.first<<"\n"; 
                    for (auto& key_value : section.second)
                    {
                        std::cout << "[+] programs  :" << section.first<<","<< key_value.first << " : " << key_value.second << std::endl;
                        this->programs.push_back(section.second[key_value.first]) ; 
                    }
                }
                else if (section.first.find("stats")!= std::string::npos)
                {
                    std::cout<<"[+]Stats found: "<<section.first<<"\n"; 
                    for (auto& key_value : section.second)
                    {
                        std::cout << "[+] stats  :" << section.first<<","<< key_value.first << " : " << key_value.second << std::endl;
                        this->stats.push_back(section.second[key_value.first]) ; 
                    }
                }
                else if (section.first.find("blocks")!= std::string::npos)
                {
                    std::cout<<"[+]Blocks found: "<<section.first<<"\n"; 
                    for (auto& key_value : section.second)
                    {
                        std::cout << "[+] blocks  :" << section.first<<","<< key_value.first << " : " << key_value.second << std::endl;
                        this->blocks.push_back(section.second[key_value.first]) ; 
                    }
                }
                else
                {
                    std::cout<<"[+]Custom found: "<<section.first<<"\n"; 
                    for (auto& key_value : section.second)
                    {
                        std::cout << "[+] custom  :" << section.first<<","<< key_value.first << " : " << key_value.second << std::endl;
                        //add to custom section
                        this->custom.push_back(section.second[key_value.first]) ;
                        
                    }
                }
                
            }
            for(auto& section : frontends)
            {
                struct front_end frontend; 
                frontend.name = section.first;
                frontend.mode = section.second["mode"];
                
                //bind will appear as bind <ip> value: <port>
                //parse the key value pair,first find any bind on section.second 
                //then split the value by space

                //any secontion.second["bind %s"] will be split by space
                //the first token will be the ip address and the second token will be the port 
                //if the bind is not found, then the default bind is 
                //
                //bind *:80
                //bind *:443 if tls enabled
                //bind *:8080 if http2 enabled  
                //bind *:8443 if http2 enabled and tls enabled

                //first find bind key in section.second
                for(auto& key_value : section.second)
                {
                    auto trimmed_key = happy_proxy::trim_space(key_value.first); 

                     if (trimmed_key.find("bind") != std::string::npos) 
                     {

                        auto tokens = happy_proxy::Tokenize(trimmed_key, " "); 

                        if(tokens.size() > 1)
                        {
                            frontend.bind = tokens[tokens.size()-1]+":"+key_value.second;

                        }
                        else
                        {
                            std::cerr<<"[+]Bind found with no tokens: "<<trimmed_key<<"\n"; 

                            frontend.bind = "  *:80";   
                        }
                     }

                }


                frontend.log = section.second["log"]; 
                frontend.maxconn = section.second["maxconn"].length()>1? std::stoi(section.second["maxconn"]):0;
                frontend.maxqueue = section.second["maxqueue"].length()>1? std::stoi(section.second["maxqueue"]):0;
                frontend.backlog = section.second["backlog"].length()>1? std::stoi(section.second["backlog"]):0;
                frontend.timeout_client = section.second["timeout client"].length()>1? std::chrono::seconds(std::stoi(section.second["timeout client"])):std::chrono::seconds(0);
                frontend.timeout_server = section.second["timeout server"].length()>1? std::chrono::seconds(std::stoi(section.second["timeout server"])):std::chrono::seconds(0);
                frontend.max_retries = section.second["max_retries"].length()>1? std::stoi(section.second["max_retries"]):0;
                frontend.retry_timeout = section.second["retry_timeout"].length()>1? std::chrono::seconds(std::stoi(section.second["retry_timeout"])):std::chrono::seconds(0);
                //parse options 
                auto options = happy_proxy::Split<std::string>(section.second["option"], " ");
                for(auto& option : options)
                {
                    frontend.options.push_back(option);
                }
                //parse acls 
                auto acls =  section.second["acl"]; 
                
                for(auto& acl : happy_proxy::Split <std::string>( acls , "\n"))
                {
                        std::cout<<"[+]Parsing Frontend ACL: "<<acl<<"\n";

                        bool allow = acl.find("allow_host") != std::string::npos;
                        bool deny =  acl.find("forbidden_dst") != std::string::npos;
                        std::string iptables = "";
                        if(allow &&!deny)
                        {
                            //src will be the next token after the allow_host src 
                            auto acl_src = happy_proxy::Split<std::string>(acl, " ").back(); 

                            iptables = "iptables -A INPUT -s "+acl_src+" -j ACCEPT";
                        }
                        else if(deny)
                        {
                            auto acl_src = happy_proxy::Split<std::string>(acl, " ").back(); 

                            iptables = "iptables -A INPUT -s "+acl_src+" -j DROP";
                        }  
                        else
                        {
                            iptables = "iptables -A INPUT -s "+section.second["bind"]+" -j ACCEPT";  
                        }
                        if(iptables.length()>1)
                            frontend.acls.push_back(iptables);

                }
               
                //parse use_backends 
                auto use_backends = happy_proxy::Split<std::string>(section.second["use_backend"], " ");
                for(auto& use_backend : use_backends)
                {
                    frontend.use_backends.push_back(use_backend);
                }
                this->_front_ends.push_back(frontend);
                //print the map

            } 
            //parse backends
            std::map<std::string, std::map<std::string, std::string>> backends; 
            for(auto& section : configuration)
            {
                if(section.first.find("backend") != std::string::npos)
                {
                    //build backend object
                    struct back_end backend; 
                    backend.name = section.first;
                    backend.mode = section.second["mode"]; 
                    backend.bind = section.second["bind"];
                    backend.log = section.second["log"];
                    backend.maxconn = section.second["maxconn"].length()>1? std::stoi(section.second["maxconn"]):0;
                    backend.maxqueue = section.second["maxqueue"].length()>1? std::stoi(section.second["maxqueue"]):0;
                    backend.backlog = section.second["backlog"].length()>1? std::stoi(section.second["backlog"]):0;
                    backend.timeout_client = section.second["timeout client"].length()>1? std::chrono::seconds(std::stoi(section.second["timeout client"])):std::chrono::seconds(0);
                    backend.timeout_server = section.second["timeout server"].length()>1? std::chrono::seconds(std::stoi(section.second["timeout server"])):std::chrono::seconds(0);
                    backend.max_retries = section.second["max_retries"].length()>1? std::stoi(section.second["max_retries"]):0;
                    backend.retry_timeout = section.second["retry_timeout"].length()>1? std::chrono::seconds(std::stoi(section.second["retry_timeout"])):std::chrono::seconds(0);
                    //parse options
                    auto options = happy_proxy::Split<std::string>(section.second["option"], " ");
                    for(auto& option : options)
                    {
                        backend.options.push_back(option);
                    }   
                    //parse acls
                    auto acls = happy_proxy::Split<std::string>(section.second["acl"], " ");
                    for(auto& acl : acls)
                    {
                        //parse application acls :
                        //valid_method, valid_http_version, valid_http_header, valid_http_content_type, valid_http_content_length 
                        //valid_http_content_encoding, valid_http_content_disposition, valid_http_content_language, valid_http_content_encoding 
                        //backend,acl l valid_method        method GET
                        //backend,http-request : t deny if !valid_method
                        
                        backend.acls.push_back(acl);
                        //backend,http-request : t deny if !valid_method
                    }   
                    //parse request_rules   
                    auto request_rules = happy_proxy::Split<std::string>(section.second["reqrep"], " ");    
                    for(auto& request_rule : request_rules)
                    {
                        backend.request_rules.push_back(request_rule);
                    }   
                    //parse response_rules  
                    auto response_rules = happy_proxy::Split<std::string>(section.second["rsprep"], " ");   
                    for(auto& response_rule : response_rules)
                    {
                        backend.response_rules.push_back(response_rule);
                    }   
                    //parse servers 
                    auto servers = happy_proxy::Split<std::string>(section.second["server"], " ");   
                    for(auto& server : servers)
                    {
                        backend.servers.push_back(server);
                    }   
                    this->_back_ends.push_back(backend);    
                    
                    std::cout<<"[+]Backend found: "<<section.first<<"\n"; 
                    backends[section.first] = section.second; 
                    //print the map
                    for(auto& key_value : section.second)
                    {
                        std::cout << "[+] backend  :" << section.first<<","<< key_value.first << " : " << key_value.second << std::endl;
                    }
                }
            }   
            
            
      } 
        std::vector<std::vector<std::string>> comments; //comments in the configuration files
        
        std::vector<std::string> global; //global section 
        std::vector<std::string> defaults; //defaults section
        std::vector<std::string> frontend; //frontend section
        std::vector<std::string> listen; //listen section
        std::vector<std::string> backend; //backend section
        std::vector<std::string> resolvers; //resolvers section
        std::vector<std::string> peers; //peers section
        std::vector<std::string> userlists; //userlists section 
        std::vector<std::string> mailers; //mailers section 
        std::vector<std::string> caches;
        std::vector<std::string> programs;
        std::vector<std::string> stats;
        std::vector<std::string> blocks;
        std::vector<std::string> custom;
   
        struct global_parameters _globals; 
        std::vector<struct front_end> _front_ends;
        std::vector<struct back_end> _back_ends;
        std::vector<struct server_details> _servers;
        std::vector<struct cache_entries> _cache_entries;

    } ProxyConfigContainer, *ProxyConfigContainerPtr; 

    struct session_details
    {
        std::string name;
        std::string ip_address;
        uint16_t port;
        std::string check;
        std::string check_interval;
        std::string check_down;
        std::string check_rise;
        std::string check_fall;
        std::string check_timeout; 
        std::string check_send;
        std::string check_expect;
        std::string check_http_send;
        std::string check_http_expect;
        std::string check_http_expect_error;
        std::string check_http_expect_nocache; 
    };
    //forward policy - can forward packets, connections or sessions .
    //also responsible for X-header insertion and removal when forwarding packets 
    //forward policy can be used to implement load balancing, failover, and session persistence 
    enum happy_proxy_forward_policy
    {
        FORWARD_PACKETS = 0,
        FORWARD_CONNECTIONS = 1,
        FORWARD_SESSIONS = 2,
        FORWARD_POLICY_MAX = 3,

    };
    //forward methods - can forward packets, connections or sessions .
    enum happy_proxy_forward_methods
    {
        FORWARD_METHOD_NETLINK = 0, //for scattered packets
        FORWARD_METHOD_SOCKET = 1, //for non TCP(UDP, ICMP, etc)
        FORWARD_METHOD_CONNECTION = 2,//for TCP connections
        FORWARD_METHOD_APPLICATION = 3, //for application sessions, HTTP Sessions and websockets 
        FORWARD_METHOD_SERVICE = 4, //for service sessions, SIP, RDP, etc 
        FORWARD_METHOD_MAX = 5,
        
    };
    enum load_balancing_proxy_methods 
    {
        RANDOM_BALANCING = 0,
        ROUND_ROBIN_BALANCING = 1,
        LEAST_CONNECTIONS_BALANCING = 2,
        SOURCE_IP_HASH_BALANCING = 3,
        URI_HASH_BALANCING = 4,
        URL_PARAM_HASH_BALANCING = 5,
        URL_COOKIE_HASH_BALANCING = 6,
        RDP_COOKIE_HASH_BALANCING = 7,
        SIP_HASH_BALANCING = 8,
        HEADER_HASH_BALANCING = 9,
        RATIO_HASH_BALANCING = 10,
        QUEUE_HASH_BALANCING = 11,
        STICK_TABLE_HASH_BALANCING = 12,
        STICKY_HASH_BALANCING = 13,
        PROVALLO_PREDICTOR_HASH_BALANCING = 14,
        DYNAMIC_RATIO_HASH_BALANCING = 15,
        LOAD_BALANCING_METHODS_MAX = 16,
    };  
    
    
    struct forwarding_parameters 
    {
        happy_proxy_forward_policy policy;
        happy_proxy_forward_methods method;
        load_balancing_proxy_methods load_balancing;

        std::string name;
        std::string description;
        std::string type;
        std::string version;
        std::string author;
        std::string license;
        std::string date;
        std::string time;
        std::string comments;
    };  
    struct proxy_forwarding
    {
        std::vector<struct forwarding_parameters> forwardings;
        //forwarding methods and parameters prioritized by policy
        std::map<happy_proxy_forward_policy, std::vector<struct forwarding_parameters>> forwardings_by_policy; 
        //forwarding methods and parameters prioritized by method
        std::map<happy_proxy_forward_methods, std::vector<struct forwarding_parameters>> forwardings_by_method; 
     };
     
    
}
 
         
#endif 
