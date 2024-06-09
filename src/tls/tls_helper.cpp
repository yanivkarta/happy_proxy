#include "tls/tls_helper.h"
#include <gnutls/crypto.h>
//for gnutls_global_init:
#include <gnutls/gnutls.h>
//for gnutls_certificate_allocate_credentials:

//for gnutls_credentials_set:
#include <gnutls/gnutls.h>
// for gnu_certificate_credentials_t:
#include <gnutls/x509.h>

#include "proxy.h"


namespace happy_proxy
{
    //helper for gnutls:
    //https://www.gnutls.org/manual/html_node/Initialising-GnuTLS.html

    // std::enable_shared_from_this<tls_globals>
    //tls_globals(const std::string& dh_params_file = "dh_params.pem", const std::string& ecdh_curve = "secp384r1", const std::string& x509_cert_file = "x509_cert.pem", const std::string& x509_key_file = "x509_key.pem", const std::string& x509_ca_file = "x509_ca.pem", const std::string& x509_dh_file = "x509_dh.pem"); 
    tls_globals::tls_globals(const std::string& dh_params_file, const std::string& ecdh_curve, const std::string& x509_cert_file, const std::string& x509_key_file, const std::string& x509_ca_file, const std::string& x509_dh_file): dh_params_file_(dh_params_file), ecdh_curve_(ecdh_curve), x509_cert_file_(x509_cert_file), x509_key_file_(x509_key_file), x509_ca_file_(x509_ca_file), x509_dh_file_(x509_dh_file)
    {
        //initialize gnutls
        gnutls_global_init();
    }
    
    //copy constructor:
    tls_globals::tls_globals(const tls_globals& other): base(other), dh_params_file_(other.dh_params_file_), ecdh_curve_(other.ecdh_curve_), x509_cert_file_(other.x509_cert_file_), x509_key_file_(other.x509_key_file_), x509_ca_file_(other.x509_ca_file_), x509_dh_file_(other.x509_dh_file_)
    {
        //initialize gnutls
        gnutls_global_init();
    }   
    tls_globals::~tls_globals()
    {
        //deinitialize gnutls
        gnutls_global_deinit();
    }

    //tls_helper
    

    tls_helper::tls_helper(const tls_globals& globals, const std::string& session): globals_(std::make_shared<tls_globals>(globals)), session_map_()
    {
        //initialize gnutls
        gnutls_init(&session_, GNUTLS_CLIENT);
        //add the session to the session map:
        std::lock_guard<std::mutex> lock(mutex_);
        session_map_[session] = session_;
    }

    tls_helper::~tls_helper()
    {
        //deinitialize gnutls
        gnutls_deinit(session_);
    }
    //    void set_x509_credentials(const std::string& cert_file, const std::string& key_file, const std::string& ca_file, const std::string& dh_file);

    void tls_helper::set_x509_credentials(const std::string& cert_file, const std::string& key_file, const std::string& ca_file, const std::string& dh_file)
    {
        //set x509 credentials
        //setup dh params
        set_dh_params(dh_file);
        //setup x509 credentials

        gnutls_certificate_credentials_t xcred; 
        //CHECK _session
        gnutls_certificate_allocate_credentials(&xcred);
        gnutls_certificate_set_x509_key_file(xcred, cert_file.c_str(), key_file.c_str(), GNUTLS_X509_FMT_PEM);
        gnutls_certificate_set_x509_trust_file(xcred, ca_file.c_str(), GNUTLS_X509_FMT_PEM);
        gnutls_credentials_set(session_, GNUTLS_CRD_CERTIFICATE, xcred);
        //deallocate the xcred:
        gnutls_certificate_free_credentials(xcred);

    }
    //set the dh params:
    void tls_helper::set_dh_params(const std::string& dh_file)
    {
        //set dh params
        auto file = dh_file.empty() ? globals_->x509_dh_file_ : dh_file;
        bool der = false;

        if(file.find(".der") != std::string::npos)
        {
            der = true;
        }

        //assume pem format:
       gnutls_x509_crt_fmt_t format = GNUTLS_X509_FMT_PEM; 
 

        gnutls_dh_params_t dh_params;
        //datum for dh params
        gnutls_datum_t dh_params_datum; 
        dh_params_datum.data = nullptr;
        dh_params_datum.size = 0;

        gnutls_dh_params_init(&dh_params);
        if ( der )
        {
            format = GNUTLS_X509_FMT_DER;
            gnutls_dh_params_import_pkcs3(dh_params, &dh_params_datum,format);
        }
        else
        {
            gnutls_dh_params_import_pkcs3(dh_params, &dh_params_datum,format);

        }   
        gnutls_credentials_set(session_, GNUTLS_CRD_IA, dh_params); 
        
        gnutls_dh_params_deinit(dh_params);

    }   
      void tls_helper::set_anon_credentials(const std::string& session)
    {
        //set anon credentials
        gnutls_anon_client_credentials_t anoncred;
        gnutls_anon_allocate_client_credentials(&anoncred);
        gnutls_credentials_set(session_, GNUTLS_CRD_ANON, anoncred);
        gnutls_anon_free_client_credentials(anoncred);

        //add the session to the session map:
        std::lock_guard<std::mutex> lock(mutex_);
        session_map_[session] = session_;
    }   
    //set the anon server credentials:
    void tls_helper::set_anon_server_credentials()
    {
        //set anon server credentials
        gnutls_anon_server_credentials_t anoncred;
        gnutls_anon_allocate_server_credentials(&anoncred);
        gnutls_credentials_set(session_, GNUTLS_CRD_ANON, anoncred);
        gnutls_anon_free_server_credentials(anoncred);
    }   
    //set the priority cache:   
    void tls_helper::set_priority_cache()
    {
        //set priority cache
        gnutls_priority_t priority_cache;
        gnutls_priority_init(&priority_cache, "NORMAL", NULL);
        gnutls_priority_set(session_, priority_cache);
        gnutls_priority_deinit(priority_cache);
    }   
    //set the session:  
    void tls_helper::set_session(gnutls_session_t session)
    {
        //set session
        session_ = session;
    }   
    //get the session:  
    gnutls_session_t tls_helper::get_session()const
    {
        //get session
        return session_;
    }   
    //get the tls globals:  
    std::shared_ptr<tls_globals> tls_helper::get_globals()const
    {
        //get tls globals
        return globals_;
    }   
    //set the x509 credentials: 
     //set the dh params:
    void tls_helper::set_dh_params()
    {
        //set dh params
        gnutls_dh_params_t dh_params;
        gnutls_x509_crt_fmt_t format = GNUTLS_X509_FMT_PEM; 
        gnutls_datum_t dh_params_datum; 
        dh_params_datum.data = nullptr;
        dh_params_datum.size = 0;

        gnutls_dh_params_init(&dh_params);
        gnutls_dh_params_import_pkcs3(dh_params, &dh_params_datum,format); 
        gnutls_credentials_set(session_, GNUTLS_CRD_IA, dh_params); 

        gnutls_dh_params_deinit(dh_params);
    }   
    //set the ecdh params:
    void tls_helper::set_ecdh_params()
    {
        //set ecdh params
        //gnutls_ecc_curve_t gnutls_curve = gnutls_str_to_ecc_curve(globals_->ecdh_curve_.c_str());
        //gnutls_ecc_set_curve(&gnutls_curve);
    }   
    //set the anon credentials: 
     //set the anon server credentials:  
  
    //set the session:  
  
    
} //happy_proxy 
// Path: src/tls/tls_helper.h

    