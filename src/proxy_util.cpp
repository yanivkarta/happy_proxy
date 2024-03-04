#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>
#include "proxy_util.h"

namespace happy_proxy
{
    std::string trim_space(const std::string& str)
    {
        //trim leading and trailing spaces,\t,\n

        std::string::size_type pos = str.find_first_not_of(" \t\n"); 
        if (pos == std::string::npos)
        {
            return str;
        }
        std::string::size_type pos2 = str.find_last_not_of(" \t\n");
        if (pos2 != std::string::npos)
        {
            return str.substr(pos, pos2 - pos + 1);
        }
        return str.substr(pos);
            
    }
    //tokenize string
    std::vector<std::string> Tokenize(const std::string& input, const std::string& delimiters)
    {
        std::vector<std::string> tokens;
        std::string::size_type lastPos = input.find_first_not_of(delimiters, 0);
        std::string::size_type pos = input.find_first_of(delimiters, lastPos);
        while (std::string::npos != pos || std::string::npos != lastPos)
        {
            tokens.push_back(input.substr(lastPos, pos - lastPos));
            lastPos = input.find_first_not_of(delimiters, pos);
            pos = input.find_first_of(delimiters, lastPos);
        }
        return tokens;
    }       
    std::string GetEnv(const std::string& name)
    {
        char* value = getenv(name.c_str());
        if (value == nullptr)
        {
            return "";
        }
        return std::string(value);
    }      
    //split: 
    std::vector<std::string> split(const std::string& input, const std::string& delimiters)  
    {
        return Split<std::string>(input, delimiters);
    }
}   

//tokenize to a container of type T