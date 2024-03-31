#include <stdalign.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>


//for event loop
//logging and error handling 
#include "util/logger.h"
#include "util/exception.h"

namespace happy_proxy 
{

    //redirect helper:

    //redirects the standard output and error to the log file

    void redirect_output(const std::string& log_file)
    {
        //open the log file
        int fd = open(log_file.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd == -1)
        {
            throw exception("Failed to open log file");
        }

        //redirect the standard output and error to the log file
        if (dup2(fd, STDOUT_FILENO) == -1 || dup2(fd, STDERR_FILENO) == -1)
        {
            throw exception("Failed to redirect output");
        }

        //close the log file
        close(fd);
    } 
    //redirect std::cout and std::cerr to syslog: 
    void redirect_output_to_syslog()
    {
        //redirect std::cout and std::cerr to syslog
        std::cout.rdbuf(new happy_proxy::streambuf(STDOUT_FILENO));
        std::cerr.rdbuf(new happy_proxy::streambuf(STDERR_FILENO));
    }   
    //initialize the logger
    void init_logger(const std::string& log_file)
    {
        //redirect the standard output and error to the log file
        redirect_output(log_file);

        //redirect std::cout and std::cerr to syslog
        redirect_output_to_syslog();
    }   
    //log message
    void log_message(const std::string& message)
    {
        //log the message
        std::cout << message << std::endl;
    }
    //log error
    void log_error(const std::string& message)
    {
        //log the error
        std::cerr << message << std::endl;
    }
    //log exception
    void log_exception(const std::exception& e)
    {
        //log the exception
        std::cerr << e.what() << std::endl;
    }   
    

}