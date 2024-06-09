#ifndef __HAPPY_PROXY_LOGGER_H_
#define __HAPPY_PROXY_LOGGER_H_

#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>
#include <fstream>
#include <iostream>
#include <syslog.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <system_error>


#include <functional>
namespace happy_proxy {
  struct level
  {
    enum pri
    {
      emerg = LOG_EMERG, 	// A panic condition
      alert = LOG_ALERT, 	// A condition that should be corrected
      critical = LOG_CRIT, 	// Critical condition, e.g, hard device error
      error = LOG_ERR, 	// Errors
      warning = LOG_WARNING, 	// Warning messages
      notice = LOG_NOTICE, 	// Possibly be handled specially
      info = LOG_INFO, 	// Informational
      debug = LOG_DEBUG 	// For debugging program
    };
  };

   class streambuf : public std::streambuf 
    {
    private:
      int fd_;
      std::string buffer_;
      std::mutex mutex_;
    public:

        streambuf(int fd) : fd_(fd) {}
        virtual ~streambuf() { sync(); }    
        virtual int_type overflow(int_type c) 
        {
          std::lock_guard<std::mutex> lock(mutex_);
          if (c != EOF) 
          {
            buffer_ += c;
          }
          if (c == EOF || buffer_.size() == 1024) 
          {
            sync();
          }
          return c;
        } 
        virtual int sync() 
        {
          std::lock_guard<std::mutex> lock(mutex_);
          if (!buffer_.empty()) 
          {
            if (write(fd_, buffer_.data(), buffer_.size()) == -1) 
            {
              throw std::system_error(errno, std::system_category());
            }
            buffer_.clear();
          }
          return 0;
        }
    };

    //ostream 
    class ostream : public std::ostream 
    { 
    private:
      streambuf buf_;
    public:
        ostream(int fd) : std::ostream(0), buf_(fd) 
        {
            rdbuf(&buf_);
        }
    };

    //redirect
    class redirect 
    {
         //c++23 support syncstream. 
    #if __cplusplus >= 202003L || defined(__cpp_lib_syncbuf)
    std::osyncstream dst;
    #else
    std::ostream dst;
    #endif

    std::ostream &src;
    std::streambuf *const sbuf;

  public:
    friend   std::ostream;
    friend   std::string;

    friend class ostream;

    redirect (std::ostream &src);
    redirect(ostream &src) ;
    
    virtual ~redirect ()
    {
      src.rdbuf (sbuf);
    }
  };

 
    //logger
    class logger
    {
    private:
      std::string name_;
      std::string ident_;
      int option_;
      int facility_;
      std::unique_ptr<streambuf> streambuf_;
      std::unique_ptr<ostream> ostream_;
      std::unique_ptr<redirect> redirect_;      
    public:
        logger(const std::string& name, const std::string& ident, int option, int facility) : name_(name), ident_(ident), option_(option), facility_(facility)
        {
            openlog(ident_.c_str(), option_, facility_);
            streambuf_ = std::make_unique<streambuf>(fileno(stderr));
            ostream_ = std::make_unique<ostream>(fileno(stderr));
            redirect_ = std::make_unique<redirect>(*ostream_);
        }
        virtual ~logger()
        {
            closelog();
        }
        
  }; //logger 
 
    
  } //namespace




#endif