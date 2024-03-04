#ifndef _HAPPY_PROXY_UTIL_H_
#define _HAPPY_PROXY_UTIL_H_

#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>


namespace happy_proxy
{
    std::string trim_space(const std::string& str);
    std::vector<std::string> Tokenize(const std::string& input, const std::string& delimiters = " "); 
    std::string GetEnv(const std::string& name); 
    //split
    template <typename T>
    std::vector<T> Split(const std::string& input, const std::string& delimiters = " ")
    {
        std::vector<T> tokens;
        std::string::size_type lastPos = input.find_first_not_of(delimiters, 0);
        std::string::size_type pos = input.find_first_of(delimiters, lastPos);
        while (std::string::npos != pos || std::string::npos != lastPos)
        {
            tokens.push_back(static_cast<T>(input.substr(lastPos, pos - lastPos)));
            lastPos = input.find_first_not_of(delimiters, pos);
            pos = input.find_first_of(delimiters, lastPos);
        }
        return tokens;
    } 
    //split
    std::vector<std::string> split(const std::string& input, const std::string& delimiters = " ");
     
}
#endif 