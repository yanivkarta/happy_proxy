#ifndef __TLS_HELPER_H_
#define __TLS_HELPER_H_

//gnutls headers
#include <gnutls/gnutls.h>
#include <gnutls/x509.h>
#include <gnutls/abstract.h>
#include <gnutls/crypto.h>
#include <gnutls/ocsp.h>
#include <gnutls/abstract.h>

//c++ headers

#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>
#include <iostream>
#include <fstream>
#include <functional>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>



namespace happy_proxy
{
  class tls_globals : public std::enable_shared_from_this<tls_globals>
  { 
    //holds the global tls settings

    //tune_ssl_default_dh_params
    //ssl_default_bind_ciphers
    //...

    //holds the global tls settings:
    //ciphers:
    //dh_params:
    //ecdh_params:
    //x509_crt:

    //dh_param file:
    std::string dh_params_file_;
    //ecdh curve:
    std::string ecdh_curve_;
    //x509 certificate file:
    std::string x509_cert_file_;
    //x509 key file:
    std::string x509_key_file_;
    //x509 ca file:
    std::string x509_ca_file_;
    //x509 dh file:
    std::string x509_dh_file_;

    //ecdh:
    //x509:
    //ssl:
    bool ssl_tune_ssl_default_dh_params_;
    std::string ssl_default_bind_ciphers_;
    std::vector<std::string> ssl_default_bind_options_; 
    std::string ssl_default_server_ciphers_;
    std::vector<std::string> ssl_default_server_options_;
    std::string ssl_default_client_ciphers_;
    std::vector<std::string> ssl_default_client_options_; 
    std::string ssl_default_client_ctx_;
    std::string ssl_default_server_ctx_;
    //verify depth:
    int ssl_default_verify_depth_;
    bool ssl_default_cache_; 
    std::string ssl_default_dh_params_;
    std::string ssl_default_ecdh_curve_;
    std::vector<std::string> ssl_default_options_;
    std::string ssl_bind_curve_;
    std::string ssl_default_bind_ciphersuites;
    std::string ssl_default_bind_ecdh_curve;

    std::string ssl_default_bind_proto_;
   
    std::string ssl_default_server_ciphersuites;
    std::string ssl_default_server_ecdh_curve ; 
    std::string ssl_default_server_proto ;
    std::string ssl_default_client_ciphersuites  ;
    std::string ssl_default_client_ecdh_curve;

    std::string ssl_default_client_proto;


    
    //gnutls global settings:
    gnutls_certificate_credentials_t x509_cred_;
    gnutls_priority_t priority_cache_;
    gnutls_dh_params_t dh_params_;
    gnutls_ocsp_resp_t ocsp_response_;
    gnutls_x509_crl_t crl_;
    gnutls_x509_crt_t cert_;
    gnutls_x509_privkey_t key_;
    gnutls_anon_client_credentials_t anoncred_; 
    gnutls_anon_server_credentials_t anon_server_cred_;
    gnutls_certificate_credentials_t xcred_;
    gnutls_certificate_credentials_t xcred_server_;
    
    gnutls_certificate_credentials_t xcred_client_;
    gnutls_certificate_credentials_t xcred_server_client_;
    //nonatomic:
    static bool initialized_; //global initialization flag, for tls_global_init 
    static std::mutex init_mutex_; //global initialization mutex, for tls_global_init
    public:
    //parameterized constructor:
    tls_globals(const std::string& dh_params_file = "dh_params.pem", const std::string& ecdh_curve = "secp384r1", const std::string& x509_cert_file = "x509_cert.pem", const std::string& x509_key_file = "x509_key.pem", const std::string& x509_ca_file = "x509_ca.pem", const std::string& x509_dh_file = "x509_dh.pem"); 
    //copy constructor:
    tls_globals(const tls_globals& other);
    //copy assignment operator:
    tls_globals& operator=(const tls_globals& other);
    //move constructor:
    tls_globals(tls_globals&& other);
    //destructor:
    ~tls_globals();
    //get the tls credentials
    gnutls_certificate_credentials_t get_x509_cred()const;
    //get the tls priority cache
    gnutls_priority_t get_priority_cache()const;
    //get the tls dh params
    gnutls_dh_params_t get_dh_params()const;
    //get the tls ocsp response
    gnutls_ocsp_resp_t get_ocsp_response()const;
    //get the tls crl
    gnutls_x509_crl_t get_crl()const;
    //get the tls cert
    gnutls_x509_crt_t get_cert()const;
    //get the tls key
    gnutls_x509_privkey_t get_key()const;
    //get the tls anoncred
    gnutls_anon_client_credentials_t get_anoncred()const;
    //get the tls anon server cred
    gnutls_anon_server_credentials_t get_anon_server_cred()const;

    //create and validate tls sessions with the tls credentials:
    gnutls_certificate_credentials_t create_xcred();
    gnutls_certificate_credentials_t create_xcred_server();
    gnutls_certificate_credentials_t create_xcred_client();
    gnutls_certificate_credentials_t create_xcred_server_client();
    //create and validate tls sessions with the tls credentials:
    gnutls_certificate_credentials_t create_xcred(const std::string& cert_file, const std::string& key_file, const std::string& ca_file, const std::string& dh_file);

    //create and validate tls sessions with the tls credentials:
    gnutls_certificate_credentials_t create_xcred_server(const std::string& cert_file, const std::string& key_file, const std::string& ca_file, const std::string& dh_file);
    //create and validate tls sessions with the tls credentials:
    gnutls_certificate_credentials_t create_xcred_client(const std::string& cert_file, const std::string& key_file, const std::string& ca_file, const std::string& dh_file);
    //create and validate tls sessions with the tls credentials:
    gnutls_certificate_credentials_t create_xcred_server_client(const std::string& cert_file, const std::string& key_file, const std::string& ca_file, const std::string& dh_file);
    
    //pin certificate:
    


    //static methods:

    static void tls_global_init();
    static void tls_global_deinit();


    //get the tls xcred

 
  };
  
  
  class tls_helper
  {

  };


}
#endif