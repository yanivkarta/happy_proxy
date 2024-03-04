#ifndef HAPPY_PROXY_EVENT_QUEUES_H
#define HAPPY_PROXY_EVENT_QUEUES_H


//include linux event queues 
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/timerfd.h>
#include <sys/signalfd.h>
#include <sys/inotify.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>

//include linux event queues

//for event loop
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

//for proxy
#include "proxy.h"
#include "proxy_config.h" 

//todo: include tls helper for dltls and tls support.
namespace happy_proxy
{
    struct async_event_param
    {
        int fd;
        int flags;
        int events;
        int timeout;
        int max_events; 
        int64_t data;
    };

     //event queues base class
     //base can be epoll, kqueue, select, poll, etc 
    class asyn_event_queues_manager
    {
        protected: 
        //epoll file descriptor, flags, events, timeout, max_events 
        async_event_param m_epoll_param;
        //kqueue file descriptor, flags, events, timeout, max_events
        //diable kqueue for now
        //async_event_param m_kqueue_param;
        async_event_param m_select_param;
        async_event_param m_poll_param;
        //event file descriptor, flags, events, timeout, max_events
        async_event_param m_event_param;
        //event base, flags, events, timeout, max_events
        async_event_param m_event_base_param;
        //callbacks: read, write, error, timeout, etc 
        event_callback_fn  m_write_cb;
        event_callback_fn  m_read_cb;
        event_callback_fn  m_error_cb;
        event_callback_fn  m_timeout_cb;
        event_callback_fn  m_signal_cb;
        event_callback_fn  m_event_cb;
        event_callback_fn  m_event_base_cb;
        
        //event queues
        int m_epoll_fd;
        //int m_kqueue_fd;
        int m_select_fd;
        int m_poll_fd;
        int m_event_fd;
        int m_signal_fd; //to block/unblock signals
        struct event_base *m_event_base;

        public:
        //constructor
        asyn_event_queues_manager( );
        //destructor
        virtual ~asyn_event_queues_manager();
        //initialize epoll
        int init_epoll( );
        //initialize kqueue
        //int init_kqueue( );
        //initialize select
        int init_select( );
        //initialize poll
        int init_poll( );
        //initialize event
        int init_event( );
        //initialize event base
        int init_event_base( );

        //add file descriptor to epoll
        int add_epoll(async_event_param *param);

        //add file descriptor to kqueue
        //int add_kqueue(async_event_param *param);

        //add file descriptor to select
        int add_select(async_event_param *param);

        //add file descriptor to poll
        int add_poll(async_event_param *param);

        //add file descriptor to event
        int add_event(async_event_param *param);

        //add file descriptor to event base
        int add_event_base(async_event_param *param);

        //remove file descriptor from epoll
        int remove_epoll(async_event_param *param);

        //remove file descriptor from kqueue
        //int remove_kqueue(async_event_param *param);

        //remove file descriptor from select
        int remove_select(async_event_param *param);

        //remove file descriptor from poll
        int remove_poll(async_event_param *param);

        //remove file descriptor from event
        int remove_event(async_event_param *param);

        //remove file descriptor from event base
        int remove_event_base(async_event_param *param);


        //wait for events on epoll
        int wait_epoll(async_event_param *param); 

        //wait for events on kqueue
        //int wait_kqueue(async_event_param *param);

        //wait for events on select
        int wait_select(async_event_param *param);

        //wait for events on poll
        int wait_poll(async_event_param *param);

        //wait for events on event
        int wait_event(async_event_param *param);

        //wait for events on event base
        int wait_event_base(async_event_param *param);

        //get epoll file descriptor
        int get_epoll_fd( );

        //get kqueue file descriptor
        //int get_kqueue_fd( );

        //get select file descriptor
        int get_select_fd( );

        //get poll file descriptor
        int get_poll_fd( );

        //get event file descriptor
        int get_event_fd( );

        //get event base
        struct event_base *get_event_base( );

        //get signal file descriptor
        int get_signal_fd( );

        
    };


    //event queues manager
    class event_queues_manager : public asyn_event_queues_manager
    {
        public:
        //constructor
        event_queues_manager( );
        //destructor
        virtual ~event_queues_manager( );
        //initialize all event queues
        int init_all( );
        //add file descriptor to all event queues
        int add_all(async_event_param *param);
        //remove file descriptor from all event queues
        int remove_all(async_event_param *param);
        //wait for events on all event queues
        int wait_all(async_event_param *param);
    };

    //proactor event queues manager 
    class proactor_event_queues_manager : public asyn_event_queues_manager
    {
        public:
        //constructor
        proactor_event_queues_manager( );
        //destructor
        virtual ~proactor_event_queues_manager( );
        //initialize all event queues
        int init_all( );
        //add file descriptor to all event queues
        int add_all(async_event_param *param);
        //remove file descriptor from all event queues
        int remove_all(async_event_param *param);
        //wait for events on all event queues
        int wait_all(async_event_param *param);
    };

    //reactor event queues manager
    class reactor_event_queues_manager : public asyn_event_queues_manager
    {
        public:
        //constructor
        reactor_event_queues_manager( );
        //destructor
        virtual ~reactor_event_queues_manager( );
        //initialize all event queues
        int init_all( );
        //add file descriptor to all event queues
        int add_all(async_event_param *param);
        //remove file descriptor from all event queues
        int remove_all(async_event_param *param);
        //wait for events on all event queues
        int wait_all(async_event_param *param);
    };  

    class io_event 
    {
        iovec m_iov;
        int m_fd;
        int m_flags;
        int m_events;
        int m_timeout;
        int m_max_events;
        public:
        io_event( );
        virtual ~io_event( );
        int init( );
        int add( );
        int remove( );
        int wait( );
        int get_fd( );
        int get_flags( );
        int get_events( );
        int get_timeout( );
        int get_max_events( );
    };
    class io_buffer_event
    {
        struct bufferevent *m_bev;
        int m_fd;
        int m_flags;
        int m_events;
        int m_timeout;
        int m_max_events;
        public:
        io_buffer_event( );
        virtual ~io_buffer_event( );
        int init( );
        int add( );
        int remove( );
        int wait( );
        struct bufferevent *get_bev( );
        int get_fd( );
        int get_flags( );
        int get_events( );
        int get_timeout( );
        int get_max_events( );
    };      
    class io_event_base
    {
        struct event_base *m_event_base;
        int m_flags;
        int m_events;
        int m_timeout;
        int m_max_events;
        public:
        io_event_base( );
        virtual ~io_event_base( );
        int init( );
        int add( );
        int remove( );
        int wait( );
        struct event_base *get_event_base( );
        int get_flags( );
        int get_events( );
        int get_timeout( );
        int get_max_events( );
    };

    class io_event_queues_manager
    {
        io_event m_io_event;
        io_buffer_event m_io_buffer_event;
        io_event_base m_io_event_base;
        public:
        io_event_queues_manager( );
        virtual ~io_event_queues_manager( );
        int init_all( );
        int add_all( );
        int remove_all( );
        int wait_all( );
    };
 

}

#endif 
