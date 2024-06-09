#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>

#include "util/exception.h"


namespace happy_proxy
{

    //exception class
    exception::~exception() {} 
    //log exception
    void exception::log_exception(const std::string& message)
    {
        std::cerr << message << std::endl;
    }
    
    //file exception
}