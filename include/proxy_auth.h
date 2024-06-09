#ifndef __PROXY_AUTH_H_
#define __PROXY_AUTH_H_

// __PROXY_AUTH_H_

//gnutls:

#include <syslog.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <gnutls/gnutls.h>
#include <gnutls/x509.h>
#include <gnutls/crypto.h>
#include <gnutls/abstract.h>

#include <string>
#include <vector>
#include <map>
#include <set>

#include <memory>
#include <mutex>
#include <thread>
#include <atomic>

#include <condition_variable>

//authentication scheme:


#include <fstream>
#include <iostream>

#include <functional>
#include <iomanip>
#include <sstream>

#include <chrono>
#include <ctime>
#include <algorithm>


#include "proxy_util.h"
#include "proxy_session_manager.h"
#include "proxy_client.h"
#include "proxy_server.h"
#include "proxy_config.h"
//for tls_globals definition:
#include "tls/tls_helper.h"


namespace happy_proxy
{

    class authentication_function
    {
    


        authentication_function(const happy_proxy::tls_globals& tls_globals);
        ~authentication_function();
        //authenticate client with username and password 
        
        bool authenticate_client(const std::string& x509_cert); 
        //authenticate client with x509 certificate and password 
        bool authenticate_client(const std::string& x509_cert, const std::string& password); 
        //authenticate client with x509 certificate and username 
        bool authenticate_client(const std::string& x509_cert, const std::string& username, const std::string& password); 
        
        //settings :
        //tls globals:
        const happy_proxy::tls_globals& tls_globals_; 
        //certificate, key, ca, dh, ecdh, ciphers, options, curve, dh_params, x509_crt, x509_key, x509_ca, x509_dh, ssl_tune_ssl_default_dh_params, ssl_default_bind_ciphers, ssl_default_bind_options, ssl_default_server_ciphers, ssl_default_server_options 
        std::string dh_params_file_;
        std::string ecdh_curve_;
        std::string x509_cert_file_;
        std::string x509_key_file_;
        std::string x509_ca_file_;
        std::string x509_dh_file_;
        bool ssl_tune_ssl_default_dh_params_;
        std::string ssl_default_bind_ciphers_; 
        std::vector<std::string> ssl_default_bind_options_; 
        std::string ssl_default_server_ciphers_; 
        std::vector<std::string> ssl_default_server_options_; 
        std::string ssl_default_client_ciphers_;
        std::vector<std::string> ssl_default_client_options_;
        std::string ssl_default_client_ctx_;
        //authentication function: 

        typedef  std::function<bool(const std::string&, const std::string&, const std::string&)>  auth_function; 
        auth_function auth_function_;
        public:
        const authentication_function& operator=(const authentication_function&); 
        authentication_function(const authentication_function&); 
        authentication_function(const happy_proxy::tls_globals& tls_globals, const std::string& dh_params_file, const std::string& ecdh_curve, const std::string& x509_cert_file, const std::string& x509_key_file, const std::string& x509_ca_file, const std::string& x509_dh_file, bool ssl_tune_ssl_default_dh_params, const std::string& ssl_default_bind_ciphers, const std::vector<std::string>& ssl_default_bind_options, const std::string& ssl_default_server_ciphers, const std::vector<std::string>& ssl_default_server_options, const std::string& ssl_default_client_ciphers, const std::vector<std::string>& ssl_default_client_options, const std::string& ssl_default_client_ctx, const auth_function& auth_function);         
    };

}
#endif 