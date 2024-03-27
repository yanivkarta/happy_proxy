#ifndef _HAPPY_PROXY_EXCEPTION_H_
#define _HAPPY_PROXY_EXCEPTION_H_

#include <exception>
#include <string>
#include <functional>
#include <signal.h>
#include <unistd.h>


#include <syslog.h>

namespace happy_proxy
{
    //exception class
    class exception : public std::exception
    {
    private:
        std::string message_;
        int error_code_;
    public:
        exception(const std::string& message, int error_code = 0) : message_(message), error_code_(error_code) {}
        virtual ~exception() ;
        virtual const char* what() const throw() { return message_.c_str(); }
        int error_code() const { return error_code_; }
        static void log_exception(const std::string& message);
    };

    //log exception
    class log_exception : public exception
    {
    public:
        log_exception(const std::string& message, int error_code = 0) : exception(message, error_code) {}
    };

    //file exception
    class file_exception : public exception
    {
    public:
        file_exception(const std::string& message, int error_code = 0) : exception(message, error_code) {}
    };

    //socket exception
    class socket_exception : public exception
    {
    public:
        socket_exception(const std::string& message, int error_code = 0) : exception(message, error_code) {}
    };  

    //network exception
    class network_exception : public exception
    {
    public:
        network_exception(const std::string& message, int error_code = 0) : exception(message, error_code) {}
    };


    //thread exception
    class thread_exception : public exception
    {
    public:
        thread_exception(const std::string& message, int error_code = 0) : exception(message, error_code) {}
    };

    //mutex exception
    class mutex_exception : public exception
    {
    public:
        mutex_exception(const std::string& message, int error_code = 0) : exception(message, error_code) {}
    };



    //signal exception
    class signal_exception : public exception
    {
    public:
        signal_exception(const std::string& message, int error_code = 0) : exception(message, error_code) {}
    };


    //process exception
    class process_exception : public exception
    {
    public:
        process_exception(const std::string& message, int error_code = 0) : exception(message, error_code) {}
    };



    //system exception
    class system_exception : public exception
    {
    public:
        system_exception(const std::string& message, int error_code = 0) : exception(message, error_code) {}
    };



    //event exception
    class event_exception : public exception
    {

    public:
        event_exception(const std::string& message, int error_code = 0) : exception(message, error_code) {}
    };

    //signal helper
    class signal_helper
    {
    public:
        static void install_handler(int signal,std::function<void(int)> handler); 

        static void install_handler(int signal, void (*handler)(int));
        static void ignore_signal(int signal);
        static void block_signal(int signal);
        static void unblock_signal(int signal);
        static void raise_signal(int signal);
    };

    //resource_cleanup helper for RAII

    template <typename T>
    class resource_cleanup
    {
    private:
        T* resource_;
        std::function<void(T*)> cleanup_ = resource_cleanup::auto_delete; 

    public:
        static void auto_delete(T* resource);
        resource_cleanup(T* resource, std::function<void(T*)> cleanup = resource_cleanup::auto_delete);
        resource_cleanup(const resource_cleanup&) = delete; 
        
        ~resource_cleanup();
    };

    //constructor:
    template <typename T>
    resource_cleanup<T>::resource_cleanup(T* resource, std::function<void(T*)> cleanup) : resource_(resource), cleanup_(cleanup)
    {
        signal_helper::install_handler(SIGABRT, [](int signal) { throw signal_exception("SIGABRT received"); }); 
        signal_helper::install_handler(SIGFPE, [](int signal) { throw signal_exception("SIGFPE received"); }); 
        signal_helper::install_handler(SIGILL, [](int signal) { throw signal_exception("SIGILL received"); }); 
        signal_helper::install_handler(SIGINT, [](int signal) { throw signal_exception("SIGINT received"); }); 
        signal_helper::install_handler(SIGSEGV, [](int signal) { throw signal_exception("SIGSEGV received"); }); 
        signal_helper::install_handler(SIGTERM, [](int signal) { throw signal_exception("SIGTERM received"); }); 
        cleanup_(resource_);
        //remove signal handlers
        signal_helper::ignore_signal(SIGABRT);
        signal_helper::ignore_signal(SIGFPE);
        signal_helper::ignore_signal(SIGILL);
        signal_helper::ignore_signal(SIGINT);
        signal_helper::ignore_signal(SIGSEGV);
        signal_helper::ignore_signal(SIGTERM);
        
    }

    //destructor:
    template <typename T>   
    
    resource_cleanup<T>::~resource_cleanup() {
        //remove signal handlers 
        signal_helper::ignore_signal(SIGABRT);
        signal_helper::ignore_signal(SIGFPE);
        signal_helper::ignore_signal(SIGILL);
        signal_helper::ignore_signal(SIGINT);
        signal_helper::ignore_signal(SIGSEGV);
        signal_helper::ignore_signal(SIGTERM);
          
        cleanup_(resource_); 
        
    } 

    //auto delete
    template <typename T>
    void resource_cleanup<T>::auto_delete(T* resource)
    {
        //install signal handler for cleanup
        
        if (resource != nullptr)
        {
            delete resource;
            resource = nullptr;
        } 
    }

    //log exception
    
} // namespace name

#endif // _HAPPY_PROXY_EXCEPTION_H_