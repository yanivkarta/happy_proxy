#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <mutex>
#include <thread>
#include "proxy.h"
//ProxyConfig implementation
#include "proxy_config.h"
#include "proxy_util.h"
#include <regex> 
namespace happy_proxy
{

     
     //all the keywords in the haproxy configuration file :

    const std::string keywords[]={"bind", "mode", "option", "acl", "server", "name", "ip", "port", "check", "stats", "stats_port", "stats_uri", "stats_user", "stats_pass",
    "stats_realm", "stats_uri", "stats_auth", "stats_admin", "stats_enable", "stats_hide_version", "stats_show_desc", "stats_show_node", "stats_show_desc", "stats_show_node", "stats_show_desc",
    "uri", "path", "stats_uri", "stats_user", "stats_pass", "stats_realm", "stats_uri", "stats_auth", "stats_admin", "stats_enable", "stats_hide_version", "stats_show_desc", "stats_show_node", "stats_show_desc", "stats_show_node", "stats_show_desc", 
    "reqadd", "reqdel", "reqrep", "rspadd", "rspdel", "rsprep", "capture", "log global", "log", "log_tag global", "log_tag", "log_format global", 
    "http-request", "http-response", "tcp-request", "tcp-response", "http-check-send", "http-check-expect", "http-check disable-on-404", "http-check expect status", "http-check expect",
    "set-dst-port","set-dst", "deny if", "allow if", "use_backend", "default_backend", "default-server", "balance", "source", "hash-type", "hash-balance factor", "hash-balance", "hash-balance url_param", "hash-balance hdr", "hash-balance hdr_val",
    "send-proxy", "send-proxy-v2","socket-stats", "socket-stats-payload", "socket-stats-payload-size", "socket-stats-payload-offset", "socket-stats-payload-length", "socket-stats-payload-dump", "socket-stats-payload-dump-size", "socket-stats-payload-dump-offset", "socket-stats-payload-dump-length", "socket-stats-payload-dump-dump", 
    "timeout", "timeout connect", "timeout client", "timeout server", "timeout http-keep-alive", "timeout http-request",    
    "timeout http-keep-alive", "timeout http-request", "timeout queue", "timeout tunnel", "timeout check", "timeout server-fin", "timeout client-fin", "timeout http-keep-alive", "timeout http-request", "timeout queue", "timeout tunnel", "timeout check", "timeout server-fin", "timeout client-fin", "timeout http-keep-alive", "timeout http-request", "timeout queue", "timeout tunnel", "timeout check", "timeout server-fin", "timeout client-fin", "timeout http-keep-alive", "timeout http-request", "timeout queue", "timeout tunnel", "timeout check", "timeout server-fin", "timeout client-fin", "timeout http-keep-alive", "timeout http-request", "timeout queue", "timeout tunnel", "timeout check", "timeout server-fin", "timeout client-fin", "timeout http-keep-alive", "timeout http-request", "timeout queue", "timeout tunnel", "timeout check", "timeout server-fin", "timeout client-fin", "timeout http-keep-alive", "timeout http-request", "timeout queue", "timeout tunnel", "timeout check", "timeout server-fin", "timeout client-fin", "timeout http-keep-alive", "timeout http-request", "timeout queue", "timeout tunnel", "timeout check", "timeout server-fin", "timeout client-fin", "timeout http-keep-alive", "timeout http-request", "timeout queue", "timeout tunnel", "timeout check", "timeout server-fin", "timeout client-fin", 
    "maxconn", "maxconnrate", "maxcomprate", "maxcompcpuusage", "maxsslconn"
    };

    proxy_config::proxy_config( const std::string& config_file_path)
    {
        //open configuration:
        //parse configuration:
        std::string sections []= {"none","global","defaults","listen","backend","frontend","acl","http","https","tcp","udp","smtp","pop3","imap","dns","dhcp","tftp","ftp","sftp","ssh","telnet","vnc","rdp","irc","xmpp","sip","rtsp","rtp","rtcp","sdp","sctp","websockets","mqtt","coap","diameter","radius","ldap","kerberos","nfs","cifs","iscsi"};

        std::map<std::string, std::map<std::string, std::string>> configuration; 

        std::cout<< "config file path : " << config_file_path << std::endl; 
       
        std::ifstream config_file(config_file_path); 
        if(config_file.is_open())
        {
            //configuration structure : 
            //section -> key value pairs 
            //key value pairs -> keys to more value pairs
            //key value pairs -> singular values

            //comments #
            //empty lines
            //sections [section]
            //key value pairs key = value
            //singular values key
            //key value pairs key value


            std::string line;
            std::string current_section="";
            std::string previous_section="";
            std::vector<std::string> sections_vec(sections, sections + sizeof(sections) / sizeof(sections[0])); 
            std::vector<std::string> comments ;
            for(auto& section : sections_vec)
            {
                configuration.insert({section, std::map<std::string, std::string>()}); 

            }
            while(std::getline(config_file, line))
            {
                //replace \t with space
                line =std::regex_replace(line, std::regex("\t"), " ");
                //trim space
                //line = trim_space(line);
                if(line.empty())
                {
                    continue;
                }
                //check for comments

                if( happy_proxy::trim_space(line)[0] == '#')
                {
                   //add to comments
                    //config is not initialized yet, add to comments 
                     comments.push_back(line);
                     continue;
                }
                //if section line is named, truncate the name and add to map before continuing  

                //check if it's a new section like defaults or global
                if(current_section.empty() )
                {
                   previous_section = current_section = ParseSectionLine( sections_vec, line); 

                   std::cout<<"[+] mapping new section : " << line << std::endl; 
                   //insert to map new section : 
                   continue;
                }
                else if(ParseSectionLine( sections_vec, trim_space( line) ) != "")
                {
                
                    
                    previous_section = current_section;
                    auto section = ParseSectionLine( sections_vec, line); 
                    if (!section.empty()) {
                    current_section = section;
                    if(current_section.length()>1)
                    {
                         //add section name (listener name, backend name, frontend name) to the section data 
                        
                         std::string name = line.substr(current_section.length()+1, line.length()); 
                         configuration[current_section].insert({"name", name});                        
                        
                    }   
                    
                        continue;
                    }
                    //else fall through to key value pair
                }
                //add to configuration 
                //add section data : 
                //append the line to the section data 
                
                auto key_index = line.find(':', 0); 
                if(key_index != std::string::npos)
                {
                    std::string key = line.substr(0, key_index); 
                    std::string value = line.substr(key_index+1, line.size()); 
                    configuration[current_section].insert({key, value}); 
                    std::cout<<"[+] mapping key value pair : " <<key<<","<<value<<", to section : " << current_section << std::endl; 
                }
                else
                {
                    //tokenize, count tokens, if 1 token, singular value, if 2 tokens, key value pair 
                    std::vector<std::string> tokens = happy_proxy::Tokenize(line, " ");
                    //remove empty/space tokens,compare with trim():
                    tokens.erase(std::remove(tokens.begin(), tokens.end(), " "), tokens.end()); 
                    if(tokens.size() == 1)
                    {
                        //singular value insert line as key. 
                        std::cout<<"[+] mapping singular value : " <<line<<", to section : " << current_section << std::endl;
                        configuration[current_section].insert({line,  ""}); //singular value insert line as key. 


                    }    
                    else if(tokens.size() == 2)
                    {
                        //key value pair insert tokens as key value pair
                        std::cout<<"[+] mapping key value pair : " <<tokens[0]<<","<<tokens[1]<<", to section : " << current_section << std::endl;
                        configuration[current_section].insert({tokens[0], tokens[1]});
                    }
                    else if(tokens.size() > 2)
                    {
                        //parse the line and insert the key value pairs 
                        std::string key = tokens[0];
                        if(key.length() > 0)
                        {
                            std::string value = line.substr(key.length(), line.length());
                            //if key exists already in the section, append the value to the key 
                            if(configuration[current_section].find(key) != configuration[current_section].end())
                            {
                                configuration[current_section][key] += "\n"+value;
                            }
                            else
                            configuration[current_section].insert({key, value});

                            std::cout<<"[+] mapping key agg value pair : " <<key<<","<<value<<", to section : " << current_section << std::endl;
                        }
                        else
                        {
                            std::cerr << "Error: could not parse key value pair" << std::endl;
                        }
                        
                    }

                    //signular value insert line as key. 
                    
                }    

                configuration[current_section].insert({line, ""}); 
            }
            config_file.close();
        }
        else
        {
            std::cerr << "Error: could not open config file" << std::endl;
        }
   
        this->config_ = new ProxyConfigContainer(configuration);
     }


    proxy_config::~proxy_config()
    {
        //delete the config container 
        if(this->config_ != nullptr)
        delete this->config_;   
    }

    void proxy_config::initialize()
    {
        //map the configuration section, frontend values, acls, backends, servers, etc to the proxy config container 
        //get the mapped values from the config container
       
         auto& servers = this->config_->_servers;
    
        //place the singular values in the server details struct
        //place the key value pairs in the server details struct


        for(auto& server :  servers)
        {
            std::cout << "Server Name: " << server.name << std::endl;
            std::cout << "Server IP: "<< server.ip_address<< std::endl;
            std::cout << "Server Port: "<< std::to_string(server.port)<< std::endl;
            std::cout << "Server Check:"<< server.check  << std::endl;
            
        }
            
        //get the mapped values from the config container
        //get the frontends, backends, servers, etc from the config container
        auto frontends = this->config_->_front_ends;
        for(auto& frontend : frontends)
        {
            std::cout << "Frontend Name: " << frontend.name << std::endl;
            std::cout << "Frontend Bind: "<< frontend.bind<< std::endl;
            std::cout << "Frontend Mode: "<< frontend.mode<< std::endl;
            for(auto& acl : frontend.acls)
            {
                std::cout << "Frontend ACL: "<< acl<< std::endl;
            }
            for(auto& option : frontend.options)
            {
                std::cout << "Frontend Option: "<< option<< std::endl;
            }
        }
        auto backends = this->config_->_back_ends;
        for(auto& backend : backends)
        {
            std::cout << "Backend Name: " << backend.name << std::endl;
            std::cout << "Backend Mode: "<< backend.mode<< std::endl;
            for(auto& server : backend.servers)
            {
                std::cout << "Backend Server: "<< server<< std::endl;
            }
        }   


    }

    ProxyConfigContainer* proxy_config::GetConfig()const
    {
        return this->config_;
    }   
    //get globals returns config_->globals:
    const struct  global_parameters& proxy_config::get_globals() const
    {
        return this->config_->_globals;
    }
    //parse section line returns the section name 
    std::string proxy_config::ParseSectionLine(const std::vector<std::string>& sections , const std::string& line) 
    {
        
        
        std::string section_name = line;
        //check for space and truncate the line 
        auto space_index = line.find(' ', 0);
        if(space_index != std::string::npos)
        {
            section_name = line.substr(0, space_index);
        }

        
        if(section_name.empty())
        {
            return "";
        }
        else
        {
            for (auto& section : sections)
            {
                if(section_name == section)
                {
                    return section_name;
                }
            }
            return "";
         }
        return section_name;

    } 


    //get frontends
    std::vector<front_end> proxy_config::get_frontends() const
    {
        //parse this->config_  frontend section and return the server details 
         //parse :  this->config_->frontend;
        //get the vector of the frontend structs : 
         return this->config_->_front_ends;


    }   
    //get backends
    std::vector<back_end> proxy_config::get_backends() const
    {
        //parse this->config_  backend section and return the server details 
        //parse :  this->config_->backend;
        //get the vector of the backend structs : 
        return this->config_->_back_ends;

    }
    //get server details
    std::vector<server_details> proxy_config::get_servers_details() const
    {
        //parse this->config_  server section and return the server details 
        //parse :  this->config_->server;
        //get the vector of the server structs : 
        return this->config_->_servers;

    }   
     //helper functions for ACLs, options, etc
     int64_t
     parse_time(const std::string& time)
    {
        
        std::regex time_regex("([0-9]+)([a-z]+)");
        std::smatch match;
         int64_t time_duration = 0ll;
        if(std::regex_search(time, match, time_regex))
        {
            std::string time_unit = match[2];
            std::string time_value = match[1];
            if(time_unit == "s")
            {
                time_duration = std::chrono::seconds(std::stoi(time_value)).count();
            }
            else if(time_unit == "ms")
            {
                time_duration = std::chrono::milliseconds(std::stoi(time_value)).count();
            }
            else if(time_unit == "us")
            {
                time_duration = std::chrono::microseconds(std::stoi(time_value)).count();
            }
            else if(time_unit == "ns")
            {
                time_duration = std::chrono::nanoseconds(std::stoi(time_value)).count();
            }
            else
            {
                time_duration = std::chrono::seconds(0).count();
            }
        }
        else
        {
            time_duration = std::chrono::seconds(0).count();
        }   
        return time_duration;
    }//parse_time
    //parse 

}
//ProxyServer implementation