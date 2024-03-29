/*Linux Kernel tls offload driver
  C++ version of https://github.com/ktls/af_ktls-tool
*/

#ifndef _KTLS_WRAPPER_H_
#define _KTLS_WRAPPER_H_

// #include <gnutls/gnutls.h>

// gnutls_transport_set_ptr
extern "C"
{
#include <linux/version.h>
#include <linux/types.h>
#include <linux/socket.h>
#include <linux/net.h>
#include <linux/tcp.h>
// TLS_SET_RECORD_TYPE , TLS_RECORD_TYPE_DATA, TLS_RECORD_TYPE_STREAM
#include <linux/tls.h>
#include <linux/sched.h>

	// CMSG_SPACE, CMSG_LEN, CMSG_DATA, CMSG_FIRSTHDR

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/net.h>

#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/timerfd.h>
#include <sys/signalfd.h>
#include <sys/poll.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <gnutls/gnutls.h>
#include <gnutls/dtls.h>
#include <gnutls/x509.h>
#include <gnutls/crypto.h>
#include <gnutls/abstract.h>
#include <gnutls/ocsp.h>
// page_size:
#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif // PAGE_SIZE
// KTLS_TAG_SIZE:
#define KTLS_TAG_SIZE 16
	// for   scatterlist:
	struct dma_addr_t
	{
		u_int64_t addr;
	};
	struct scatterlist
	{
		u_int64_t page_link;
		u_int32_t offset;
		u_int32_t length;
		dma_addr_t dma_address;
		// length and flags if compiled with CONFIG_DEBUG_SG
		u_int32_t orig_nents;
		u_int32_t nents;
	}; // scatterlist

	// for struct kvec:
	struct kvec
	{
		void *iov_base;
		size_t iov_len;
	};

// for struct af_alg_sgl:
#include <linux/if_alg.h>
	struct af_alg_sgl
	{
		struct scatterlist *sgl;
		size_t len;
	};
}

#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
// c++ headers
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
#include <chrono>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <sys/socket.h>
#include <netinet/in.h>

#define PF_KTLS 12
#define AF_KTLS PF_KTLS

#define KTLS_CIPHER_AES_GCM_128 1
#define KTLS_CIPHER_AES_GCM_256 2
#define KTLS_CIPHER_CHACHA20_POLY1305 3

#define KTLS_VERSION_1_2 1
#define KTLS_VERSION_1_3 2

#define KTLS_SET_TX_KEY 1
#define KTLS_SET_RX_KEY 2
#define KTLS_SET_TX_IV 3
#define KTLS_SET_RX_IV 4
#define KTLS_SET_TX_SALT 5
#define KTLS_SET_RX_SALT 6
#define KTLS_SET_TX_SEQ 7
#define KTLS_SET_RX_SEQ 8
#define KTLS_SET_TX_MODE 9
#define KTLS_SET_RX_MODE 10
#define KTLS_SET_TX_RECORD_SIZE 11
#define KTLS_SET_RX_RECORD_SIZE 12
#define KTLS_SET_TX_RECORD_TYPE 13
#define KTLS_SET_RX_RECORD_TYPE 14
#define KTLS_SET_TX_MAC 15
#define KTLS_SET_RX_MAC 16
#define KTLS_SET_TX_MAC_SIZE 17
#define KTLS_SET_RX_MAC_SIZE 18
#define KTLS_SET_TX_MAC_KEY 19
#define KTLS_SET_RX_MAC_KEY 20

#define KTLS_SET_MTU 21
#define KTLS_SET_TX_OFFLOAD 22
#define KTLS_SET_RX_OFFLOAD 23
#define KTLS_SET_TX_PUSH 24
#define KTLS_SET_RX_PUSH 25
#define KTLS_SET_TX_PUSH_SIZE 26

#define KTLS_AES_GCM_128_IV_SIZE ((size_t)8)
#define KTLS_AES_GCM_128_KEY_SIZE ((size_t)16)
#define KTLS_AES_GCM_128_SALT_SIZE ((size_t)4)

#define KTLS_AES_GCM_256_IV_SIZE ((size_t)8)
#define KTLS_AES_GCM_256_KEY_SIZE ((size_t)32)
#define KTLS_AES_GCM_256_SALT_SIZE ((size_t)4)

#define KTLS_CHACHA20_POLY1305_IV_SIZE ((size_t)8)
#define KTLS_CHACHA20_POLY1305_KEY_SIZE ((size_t)32)
#define KTLS_CHACHA20_POLY1305_SALT_SIZE ((size_t)4)

#define KTLS_MAX_PAYLOAD_SIZE ((size_t)0x3FFF)
#define KMAX(a, b) ((a) > (b) ? (a) : (b))

#define KTLS_PROTO_OPENCONNECT 128
#define KTLS_PROTO_OPENVPN 129
#define KTLS_PROTO_WIREGUARD 130
#define KTLS_PROTO_MAX 255
#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif // CHAR_BIT
#define KTLS_RECORD_DATA 0x17
#define KTLS_KEY_SIZE KTLS_AES_GCM_128_KEY_SIZE
#define KTLS_SALT_SIZE KTLS_AES_GCM_128_SALT_SIZE
#define KTLS_IV_SIZE KTLS_AES_GCM_128_IV_SIZE
#define KTLS_MAX_KEY_SIZE KTLS_AES_GCM_128_KEY_SIZE

#define KTLS_NONCE_SIZE 8
#define KTLS_DATA_PAGES (KTLS_MAX_PAYLOAD_SIZE / PAGE_SIZE)
#define KTLS_VEC_SIZE (KTLS_DATA_PAGES + 2)
#define KTLS_SG_DATA_SIZE (KTLS_DATA_PAGES + 3)
#define KTLS_AAD_SPACE_SIZE 24
#define KTLS_PADDED_AAD_SIZE 21
#define KTLS_AAD_SIZE 13
#define KTLS_TLS_HEADER_SIZE 5
#define KTLS_TLS_PREPEND_SIZE (KTLS_TLS_HEADER_SIZE + KTLS_NONCE_SIZE)
#define KTLS_TLS_OVERHEAD (KTLS_TLS_PREPEND_SIZE + KTLS_TAG_SIZE)

#define KTLS_TLS_1_2_MAJOR 0x03
#define KTLS_TLS_1_2_MINOR 0x03

#define KTLS_TLS_1_3_MAJOR 0x03
#define KTLS_TLS_1_3_MINOR 0x04
#define KTLS_TLS_NONCE_OFFSET KTLS_TLS_HEADER_SIZE
#define KTLS_DTLS_HEADER_SIZE 13

#define KTLS_DTLS_PREPEND_SIZE (KTLS_DTLS_HEADER_SIZE + KTLS_NONCE_SIZE)

#define KTLS_DTLS_OVERHEAD (KTLS_DTLS_PREPEND_SIZE + KTLS_TAG_SIZE)
#define KTLS_DTLS_1_2_MAJOR 0xFE
#define KTLS_DTLS_1_2_MINOR 0xFD
#define KTLS_DTLS_1_3_MAJOR 0xFE
#define KTLS_DTLS_1_3_MINOR 0xFE
#define KTLS_DTLS_1_3_VERSION 0x7F

#define KTLS_DTLS_SEQ_NUM_OFFSET 3
#define KTLS_DTLS_SEQ_NUM_SIZE 8
#define KTLS_DTLS_SEQ_NUM_MAX 0xFFFFFFFFFFFFFFFFULL
#define KTLS_DTLS_NONCE_OFFSET KTLS_DTLS_HEADER_SIZE
#define KTLS_SETSOCKOPT_READY(T) ((T)->aead_send != NULL && (T)->aead_recv != NULL)
#define KTLS_GETSOCKOPT_READY(T) KTLS_SETSOCKOPT_READY(T)
#define KTLS_SEND_READY(T) ((T)->key_send.keylen &&  \
							(T)->key_send.saltlen && \
							(T)->iv_send &&          \
							KTLS_GETSOCKOPT_READY(T))

#define KTLS_RECV_READY(T) ((T)->key_recv.keylen &&  \
							(T)->key_recv.saltlen && \
							(T)->iv_recv &&          \
							KTLS_GETSOCKOPT_READY(T))

#define DTLS_EPOCH_SHIFT (6 * CHAR_BIT)

#define DTLS_SEQ_NUM_MASK 0x0000FFFFFFFFFFFF
#define DTLS_WINDOW_INIT(W) ((W).bits = (W.start) = 0)
#define DTLS_SAME_EPOCH(S1, S2) (((S1) >> DTLS_EPOCH_SHIFT) == ((S2) >> DTLS_EPOCH_SHIFT))
#define DTLS_WINDOW_INSIDE(W, S) ((((S) & DTLS_SEQ_NUM_MASK) > (W).start) && \
								  (((S) & DTLS_SEQ_NUM_MASK) - (W).start <= (sizeof((W).bits) * CHAR_BIT)))
#define DTLS_WINDOW_OFFSET(W, S) ((((S) & DTLS_SEQ_NUM_MASK) - (W).start) - 1)

#define DTLS_WINDOW_RECEIVED(W, S) (((W).bits & ((u64)1 << DTLS_WINDOW_OFFSET(W, S))) != 0)

#define DTLS_WINDOW_MARK(W, S) ((W).bits |= ((u64)1 << DTLS_WINDOW_OFFSET(W, S)))

#define DTLS_WINDOW_UPDATE(W)     \
	while ((W).bits & (u64)1)     \
	{                             \
		(W).bits = (W).bits >> 1; \
		(W).start++;              \
	}

#define IS_TLS(T) (T->tls)
#define IS_DTLS(T) (!IS_TLS(T))
#define KTLS_RECORD_SIZE(T, S) (IS_TLS(T) ? (S + KTLS_TLS_OVERHEAD) : (S + KTLS_DTLS_OVERHEAD))

/*
 * Nonce explicit offset in a record
 */
#define KTLS_NONCE_OFFSET(T) (IS_TLS(T) ? (KTLS_TLS_NONCE_OFFSET) : (KTLS_DTLS_NONCE_OFFSET))

#define TLS_CACHE_DISCARD(T) (T->recv_occupied = 0)
#define TLS_CACHE_SIZE(T) (T->recv_occupied)
#define TLS_CACHE_SET_SIZE(T, S) (T->recv_occupied = S)

#define OPENCONNECT_PKT_DATA 0x00
#define OPENCONNECT_PKT_COMPRESSED 0x01

#define KTLS_SET_RECORD_TYPE 1
#define KTLS_SET_SALT_SEND 2
#define KTLS_SET_SALT_RECV 3
#define KTLS_SET_KEY_SEND 4
#define KTLS_SET_IV_RECV 5
#define KTLS_SET_IV_SEND 6
#define KTLS_SET_KEY_RECV 7
struct sockaddr_ktls
{
	sa_family_t sa_family;
	unsigned int sa_cipher;
	unsigned int sa_socket;
	unsigned int sa_version;
	unsigned int sa_reserved[8];
};

struct tls_key
{
	char *key;
	size_t keylen;
	char salt[KTLS_SALT_SIZE];
	size_t saltlen;
};
// work struct
struct work_struct
{
	void (*func)(struct work_struct *work);
	struct work_struct *next;
};

struct tls_sock
{
	/* struct sock must be the very first member */
	struct sock
	{
		/* struct sock must be the very first member */
		struct socket *sk_socket;
	} sk;
	/*
	 * TCP/UDP socket we are binded to
	 */
	struct socket *socket;

	/*
	 * Context for {set,get}sockopt()
	 */
	char *iv_send;
	struct tls_key key_send;

	char *iv_recv;
	struct tls_key key_recv;

	struct crypto_aead *aead_send;
	struct crypto_aead *aead_recv;

	/*
	 * Sending context
	 */
	scatterlist sg_tx_data[KTLS_SG_DATA_SIZE];
	kvec vec_send[KTLS_VEC_SIZE];
	char header_send[KMAX(KTLS_TLS_PREPEND_SIZE, KTLS_DTLS_PREPEND_SIZE)];
	char aad_send[KTLS_AAD_SPACE_SIZE];
	char tag_send[KTLS_TAG_SIZE];
	struct page *pages_send;
	struct af_alg_sgl sgl_send;
	struct scatterlist sgaad_send[2];
	struct scatterlist sgtag_send[2];

	/*
	 * Receiving context, rx_lock has to be acquired before socket lock to
	 * avoid deadlock
	 */
	std::mutex rx_lock;
	struct scatterlist sg_rx_data[KTLS_SG_DATA_SIZE];
	struct kvec vec_recv[KTLS_VEC_SIZE];
	char header_recv[KMAX(KTLS_TLS_PREPEND_SIZE, KTLS_DTLS_PREPEND_SIZE)];
	char aad_recv[KTLS_AAD_SPACE_SIZE];
	char tag_recv[KTLS_TAG_SIZE];
	struct page *pages_recv;
	struct af_alg_sgl sgl_recv;
	struct scatterlist sgaad_recv[2];
	struct scatterlist sgtag_recv[2];

	/*
	 * Asynchronous work to cache one record
	 */
	struct work_struct recv_work;
	void (*saved_sk_data_ready)(struct sock *sk);
	struct scatterlist sg_rx_async_work[KTLS_SG_DATA_SIZE];
	struct page *pages_work;
	size_t recv_occupied;

	/*
	 * our cipher type and its crypto API representation (e.g. "gcm(aes)")
	 */
	unsigned cipher_type;
	char *cipher_crypto;

	/*
	 * TLS/DTLS version for header
	 */
	char version[2];

	/*
	 * nonzero if TLS, zero for DTLS
	 */
	int tls;

	/*
	 * additional options, e.g. OpenConnect protocol
	 */
	unsigned opts;

	/*
	 * store mtu for raw payload -- without header, tag, (and seq num
	 * when DTLS)
	 */
	size_t mtu_payload;

	/*
	 * DTLS window handling, see DTLS_WINDOW_* macros
	 */
	struct
	{
		uint64_t bits;
		unsigned start;
	} dtls_window;

	/*
	 * Context used for sendpage(2) packetization
	 */
	struct
	{
		struct scatterlist sg[KTLS_SG_DATA_SIZE];
		size_t used;
		size_t current_size;
		size_t desired_size;
		struct page *page;
	} sendpage_ctx;
};

namespace happy_proxy
{
	namespace tls
	{

		class ktls_session
		{
			struct workqueue_struct *tls_wq;

			gnutls_session_t session_;
			int sd_;
			size_t sendfile_mtu_;
			bool tls_offload_;

		public:
			gnutls_session_t session() const { return session_; }
			int sd() const { return sd_; }
			size_t sendfile_mtu() const { return sendfile_mtu_; }
			bool tls_offload() const { return tls_offload_; }
			ktls_session(gnutls_session_t session, int sd, size_t sendfile_mtu, bool tls_offload) : session_(session), sd_(sd), sendfile_mtu_(sendfile_mtu), tls_offload_(tls_offload) {}
			~ktls_session() { gnutls_deinit(session_); }
		};
		class ktls_wrapper
		{

		public:
			ktls_wrapper() {}
			~ktls_wrapper() {}
			int ktls_socket(int domain, int type, int protocol)
			{
				//use raw socket 
				
				int ret = socket(domain, type, protocol); 


				if (ret >= 0)
				{
					// set sockopt for ktls
					int optval = 1;
					if (setsockopt(ret, SOL_TLS, TLS_TX, &optval, sizeof(optval)) < 0)
					{
						std::cerr << "[-]failed to setsockopt for ktls" << std::endl; 
						close(ret);
						return -1;
					}
					optval = 1;
					// set sockopt for ktls
					if (setsockopt(ret, SOL_TLS, TLS_RX, &optval, sizeof(optval)) < 0)
					{
						std::cerr<<"[-]failed to setsockopt for ktls" << std::endl;	
						close(ret);
						return -1;
					}
				}
				return ret;
			}
			int ktls_socket(gnutls_session_t session, int domain, int type, int protocol)
			{
				int ret = socket(domain, type, protocol);
				if (ret > 0)
				{
					// set sockopt for ktls
					int optval = 1;
					if (setsockopt(ret, SOL_TLS, TLS_TX, &optval, sizeof(optval)) < 0)
					{
						std::cerr<<"[-]failed to setsockopt for ktls" << std::endl; 
						close(ret);
						return -1;
					}
					optval = 1;
					// set sockopt for ktls
					if (setsockopt(ret, SOL_TLS, TLS_RX, &optval, sizeof(optval)) < 0)
					{
						// setsockopt failed
						std::cerr<<"[-]failed to setsockopt for ktls" << std::endl;
						close(ret);
						return -1;
					}
					// gnutls_transport_set_ptr :
					gnutls_transport_set_ptr(session, (gnutls_transport_ptr_t)&ret);
					if (ret < 0)
					{
						// gnutls_transport_set_ptr failed 
						std::cerr<<"[-]failed to set gnutls_transport_set_ptr" << std::endl; 
						close(ret);
						return -1;
					}

					// set the session
					if (setsockopt(ret, SOL_TLS, TLS_SET_RECORD_TYPE, &session, sizeof(session)) < 0)
					{
						// setsockopt failed 
						std::cerr<<"[-]failed to setsockopt for ktls session" << std::endl; 
						close(ret);
						return -1;
					}
					// complete the handshake
					if (gnutls_handshake(session) < 0)
					{
						// handshake failed
						std::cerr<<"[-]failed to complete the handshake" << std::endl; 
						close(ret);
						return -1;
					}
				}
				return ret;
			}

			// update state:
			int ktls_socket_update_state(gnutls_session_t session, int ksd, bool tls)
			{
				int err;
				gnutls_datum_t mac_key;
				gnutls_datum_t iv_read;
				gnutls_datum_t iv_write;
				gnutls_datum_t cipher_key_read;
				gnutls_datum_t cipher_key_write;
				unsigned char seq_number_read[8];
				unsigned char seq_number_write[8];

				// now we need to initialize state after the handshake in the kernel
				err = gnutls_record_get_state(session, 1, &mac_key, &iv_read, &cipher_key_read, seq_number_read);
				if (err < 0)
				{
					perror("failed to get receiving state from Gnu TLS session");
					return err;
				}

				err = gnutls_record_get_state(session, 0, &mac_key, &iv_write, &cipher_key_write, seq_number_write);
				if (err < 0)
				{
					perror("failed to get sendig state from Gnu TLS session");
					return err;
				}

				err = setsockopt(ksd, AF_KTLS, KTLS_SET_SALT_SEND, iv_write.data, 4);
				if (err < 0)
				{
					perror("failed to set send salt on AF_KTLS socket using setsockopt(2)");
					return err;
				}

				err = setsockopt(ksd, AF_KTLS, KTLS_SET_SALT_RECV, iv_read.data, 4);
				if (err < 0)
				{
					perror("failed to set recv salt on AF_KTLS socket using setsockopt(2)");
					return err;
				}

				err = setsockopt(ksd, AF_KTLS, KTLS_SET_KEY_SEND, cipher_key_write.data, cipher_key_write.size);
				if (err < 0)
				{
					perror("failed to set send key on AF_KTLS socket using setsockopt(2)");
					return err;
				}

				err = setsockopt(ksd, AF_KTLS, KTLS_SET_KEY_RECV, cipher_key_read.data, cipher_key_read.size);
				if (err < 0)
				{
					perror("failed to set receive key on AF_KTLS socket using setsockopt(2)");
					return err;
				}

				err = setsockopt(ksd, AF_KTLS, KTLS_SET_IV_SEND, seq_number_write, 8);
				if (err < 0)
				{
					perror("failed to set send IV on AF_KTLS socket using setsockopt(2)");
					return err;
				}

				/*
				 * Gnu TLS this is a workaround since Gnu TLS does not propagate recv seq num
				 * for DTLS.
				 * It should be fixed in the new release (today is Apr 1 2016). Once fixed,
				 * this has to be removed.
				 */
				if (!tls)
				{
					seq_number_read[1] = 1;
					seq_number_read[7] = 1;
				}

				err = setsockopt(ksd, AF_KTLS, KTLS_SET_IV_RECV, seq_number_read, 8);
				if (err < 0)
				{
					perror("failed to set receive IV on AF_KTLS socket using setsockopt(2)");
					return err;
				}

				return 0;
			}

			int ktls_socket_init(gnutls_session_t session, int sd, size_t sendfile_mtu, bool tls)
			{
				int err;
				struct sockaddr_ktls sa_ktls;

				int ksd = socket(AF_KTLS, tls ? SOCK_STREAM : SOCK_DGRAM, 0);
				if (ksd == -1)
				{
					perror("socket error:");
					return -1;
				}

				sa_ktls.sa_cipher = KTLS_CIPHER_AES_GCM_128;
				sa_ktls.sa_socket = sd; // bind to this socket
				sa_ktls.sa_version = KTLS_VERSION_1_2;
				sa_family_t family = tls ? AF_INET : AF_INET6; 
				sa_ktls.sa_family = family;

				

				err = bind(ksd, (struct sockaddr *)&sa_ktls, sizeof(sa_ktls));
				if (err < 0)
				{
					perror("failed to bind TCP/UCP socket");
					close(ksd);
					return -1;
				}

				err = ktls_socket_update_state(session, ksd, tls);
				if (err < 0)
				{
					close(ksd);
					return -1;
				}

				if (sendfile_mtu)
				{
					err = setsockopt(ksd, AF_KTLS, KTLS_SET_MTU, &sendfile_mtu, sizeof(sendfile_mtu));
					if (err < 0)
					{
						perror("setsockopt");
						perror("failed to set MTU on AF_KTLS socket using setsockopt(2)");
						close(ksd);
						return -1;
					}
				}

				return ksd;
			}
		};
	} // namespace tls
} // namespace happy_proxy

// namespace happy_proxy

#endif
