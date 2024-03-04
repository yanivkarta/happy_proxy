#include "tls_helper.h"
#include "proxy.h"


namespace happy_proxy
{
    //helper for gnutls:
    //https://www.gnutls.org/manual/html_node/Initialising-GnuTLS.html

tls_helper::tls_helper():priority_("NORMAL"),cert_file_(""),key_file_(""),ca_file_(""),dh_file_(""),ocsp_file_(""),crl_file_(""),verify_flags_(""),verify_depth_(9),
    x509_cred_(NULL),priority_cache_(NULL),dh_params_(NULL),ocsp_response_(NULL),verify_status_(0),ocsp_status_(0),ocsp_response_data_({0}),ocsp_resp_(NULL),crl_data_({0}),crl_(NULL),cert_(NULL),key_(NULL),session_(NULL),anoncred_(NULL),anon_server_cred_(NULL),xcred_(NULL),xcred_server_(NULL),xcred_client_(NULL),xcred_server_client_(NULL)
    {
        //initialize gnutls
        gnutls_global_init();
        gnutls_certificate_allocate_credentials(&this->x509_cred_);
        gnutls_priority_init(&this->priority_cache_, this->priority_.c_str(), NULL);
        gnutls_certificate_set_x509_trust_file(this->x509_cred_, this->ca_file_.c_str(), GNUTLS_X509_FMT_PEM);
        gnutls_certificate_set_x509_key_file(this->x509_cred_, this->cert_file_.c_str(), this->key_file_.c_str(), GNUTLS_X509_FMT_PEM);
        gnutls_certificate_set_verify_flags(this->x509_cred_, this->verify_flags_);
        gnutls_certificate_set_verify_limits(this->x509_cred_, this->verify_depth_);
        gnutls_dh_params_init(&this->dh_params_);
        gnutls_dh_params_generate2(this->dh_params_, 1024);
        gnutls_certificate_set_dh_params(this->x509_cred_, this->dh_params_);
        //gnutls_ocsp_init(&this->ocsp_response_);
        //initialize ocsps:
        gnutls_ocsp_resp_init(&this->ocsp_resp_);

        gnutls_x509_crl_init(&this->crl_);
        gnutls_x509_crt_init(&this->cert_);
        gnutls_x509_privkey_init(&this->key_);
        gnutls_anon_allocate_client_credentials(&this->anoncred_);
        gnutls_anon_allocate_server_credentials(&this->anon_server_cred_);
        gnutls_certificate_allocate_credentials(&this->xcred_);
        gnutls_certificate_allocate_credentials(&this->xcred_server_);
        gnutls_certificate_allocate_credentials(&this->xcred_client_);
        gnutls_certificate_allocate_credentials(&this->xcred_server_client_);
    }
    
    tls_helper::~tls_helper()
    {
        gnutls_certificate_free_credentials(this->x509_cred_);
        gnutls_priority_deinit(this->priority_cache_);
        gnutls_dh_params_deinit(this->dh_params_);
        gnutls_ocsp_resp_deinit(this->ocsp_resp_);
      
        gnutls_x509_crl_deinit(this->crl_);
        gnutls_x509_crt_deinit(this->cert_);
        gnutls_x509_privkey_deinit(this->key_);
        gnutls_anon_free_client_credentials(this->anoncred_);
        gnutls_anon_free_server_credentials(this->anon_server_cred_);
        gnutls_certificate_free_credentials(this->xcred_);
        gnutls_certificate_free_credentials(this->xcred_server_);
        gnutls_certificate_free_credentials(this->xcred_client_);
        gnutls_certificate_free_credentials(this->xcred_server_client_);
        gnutls_global_deinit(); 
    }
    //get the tls credentials
     

}

    