//c headers
extern "C" {
#include <stdio.h>

#include <sys/eventfd.h>

#include <sys/select.h>
//for poll
#include <poll.h>
//for epoll
#include <sys/epoll.h>
//for event
//for EVFILT_READ and EV_ADD
#include <sys/types.h>

#include <linux/version.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <event.h> //for libevent
#include <fcntl.h>
#include <unistd.h>
#include <linux/kernel.h>
#include <linux/userio.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/eventfd.h>
#include <sys/timerfd.h>
#include <sys/signalfd.h>
#include <sys/inotify.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/version.h>
#include <linux/userio.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> 
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

}
#include "proxy_ev_queues.h"
#include "proxy.h"
//for kqueue
 //for select
#include "proxy.h"
#include "proxy_config.h"
#include "proxy_ev_queues.h"
#include "proxy.h"
#include "proxy_config.h"
#include "aio/aio_control.h"


extern "C" int kqueue(void);
extern "C" int kevent(int kq, const struct kevent *changelist, int nchanges, struct kevent *eventlist, int nevents, const struct timespec *timeout); 
extern "C" int epoll_create1(int flags);
extern "C" int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
extern "C" int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
extern "C" int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
extern "C" int poll(struct pollfd *fds, nfds_t nfds, int timeout);
extern "C" int eventfd(unsigned int initval, int flags);
extern "C" int timerfd_create(int clockid, int flags);
extern "C" int timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);
extern "C" int signalfd(int fd, const sigset_t *mask, int flags);
extern "C" int inotify_init(void);
extern "C" int inotify_add_watch(int fd, const char *pathname, uint32_t mask);
extern "C" int inotify_rm_watch(int fd, int wd);

extern "C" int eventfd(unsigned int initval, int flags);
extern "C" int timerfd_create(int clockid, int flags);
extern "C" int timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);
extern "C" int signalfd(int fd, const sigset_t *mask, int flags);
extern "C" int inotify_init(void);
extern "C" int inotify_add_watch(int fd, const char *pathname, uint32_t mask);
extern "C" int inotify_rm_watch(int fd, int wd);
extern "C" int eventfd(unsigned int initval, int flags);
extern "C" int timerfd_create(int clockid, int flags);
extern "C" int timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);
extern "C" int signalfd(int fd, const sigset_t *mask, int flags);
extern "C" int inotify_init(void);
extern "C" int inotify_add_watch(int fd, const char *pathname, uint32_t mask);
extern "C" int inotify_rm_watch(int fd, int wd);
extern "C" event_base *event_base_new(void);
extern "C" int event_base_dispatch(struct event_base *base);
extern "C" int event_base_loop(struct event_base *base, int flags);
extern "C" int event_base_loopexit(struct event_base *base, const struct timeval *tv);
extern "C" int event_base_loopbreak(struct event_base *base);
extern "C" int event_base_got_exit(struct event_base *base);
extern "C" int event_base_got_break(struct event_base *base);
extern "C" void event_base_free(struct event_base *base);
 

namespace happy_proxy { 
    using namespace std;
    using namespace happy_proxy;

//asnyc event queues base class 
//base can be epoll, kqueue, select, poll, etc
asyn_event_queues_manager::asyn_event_queues_manager( ) {
    m_epoll_fd = -1;
    //m_kqueue_fd = -1;
    m_select_fd = -1;
    m_poll_fd = -1;
    m_event_fd = -1;
    m_signal_fd = -1;
    m_event_base = NULL;
}

asyn_event_queues_manager::~asyn_event_queues_manager() {
    if (m_epoll_fd > 0) {
        close(m_epoll_fd);
    }
    //if (m_kqueue_fd > 0) {
    //    close(m_kqueue_fd);
    //}
    if (m_select_fd > 0) {
        close(m_select_fd);
    }
    if (m_poll_fd > 0) {
        close(m_poll_fd);
    }
    if (m_event_fd > 0) {
        close(m_event_fd);
    }
    if (m_signal_fd > 0) {
        close(m_signal_fd);
    }
    if (m_event_base) {
        event_base_free(m_event_base);
    }

}

int asyn_event_queues_manager::init_epoll( ) {
    m_epoll_fd = epoll_create1(0);
    if (m_epoll_fd == -1) {
        perror("epoll_create1");
        return -1;
    }
    return 0;
}
 

int asyn_event_queues_manager::init_select( ) {
    //create eventfd for select
    m_select_fd = eventfd(0, 0);
    if (m_select_fd == -1) {
        perror("eventfd");
        return -1;
    }
    return 0;
}

int asyn_event_queues_manager::init_poll( ) {
    //create eventfd for poll
    m_poll_fd = eventfd(0, 0);
    if (m_poll_fd == -1) {
        perror("eventfd");
        return -1;
    }
    return 0;

} //init_poll

//init event
int asyn_event_queues_manager::init_event( ) {
    //create eventfd for event
    m_event_fd = eventfd(0, 0);
    if (m_event_fd == -1) {
        perror("eventfd");
        return -1;
    }
    return 0;   

} //init_event

//init event base
int asyn_event_queues_manager::init_event_base( ) {
    m_event_base = event_base_new();
    if (m_event_base == NULL) {
        perror("event_base_new");
        return -1;
    }
    return 0;   
} //init_event_base

//add file descriptor to epoll  
int asyn_event_queues_manager::add_epoll(async_event_param *param) {
    struct epoll_event ev;
    ev.events = param->events;
    ev.data.fd = param->fd;
    if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, param->fd, &ev) == -1) {
        perror("epoll_ctl");
        return -1;
    }
    return 0;   
} //add_epoll
 

//add file descriptor to select
int asyn_event_queues_manager::add_select(async_event_param *param) {
    
    //m_select_fd is the eventfd for select 
    m_poll_param.fd = m_select_fd;
    m_poll_param.events = POLLIN;
    
    pollfd fds[1];
    fds[0].fd = m_select_fd;
    fds[0].events = POLLIN;
    fds[0].revents = param->events;

    if (poll(fds, 1, 0) == -1) {
        perror("poll");
        return -1;
    }
    return 0;


} //add_select

//add file descriptor to poll
int asyn_event_queues_manager::add_poll(async_event_param *param) {
    //m_poll_fd is the eventfd for poll 
    m_poll_param.fd = m_poll_fd;
    m_poll_param.events = POLLIN;
    pollfd fds[1];
    fds[0].fd = m_poll_fd;
    fds[0].events = POLLIN;
    fds[0].revents = param->events;
    
    if (poll(fds, 1, 0) == -1) {
        perror("poll");
        return -1;
    }
    return 0;

} //add_poll    
//add file descriptor to epoll
int asyn_event_queues_manager::add_event(async_event_param *param) {
    //m_event_fd is the eventfd for event 
       epoll_event event;
    event.data.fd = m_event_fd;
    event.events = EPOLLIN;
    event.data.ptr = param;
    
    //epoll_ctl param:
    //int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event); 


    if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, m_event_fd, &event) == -1) {
        perror("epoll_ctl");
        return -1;
    }
    return 0;


} //add_event   

//add file descriptor to event base 
//remove file descriptor from epoll
int asyn_event_queues_manager::remove_epoll(async_event_param *param) {
    if (epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, param->fd, NULL) == -1) {
        perror("epoll_ctl");
        return -1;
    }
    return 0;

} //remove_epoll    
 //remove file descriptor from select
int asyn_event_queues_manager::remove_select(async_event_param *param) {
    //m_select_fd is the eventfd for select 
    
    pollfd fds[1];
    fds[0].fd = param->fd;
    fds[0].events = POLLIN;
    fds[0].revents = 0;
    if (poll(fds, 1, 0) == -1) {
        perror("poll");
        return -1;
    }
    return 0;

} //remove_select   
//remove file descriptor from poll  
int asyn_event_queues_manager::remove_poll(async_event_param *param) {
    //m_poll_fd is the eventfd for poll 
    if(this->m_poll_fd <= 0) {
        return -1;
    }
    else
    {
        eventfd_t value;
        if (eventfd_read(param->fd, &value) == -1) {
            perror("eventfd_read");
            return -1;
        }

        else if (value == 0) {
            return -1;
        }

        return (int) value;

    }
    

} //remove_poll 

//remove file descriptor from event
int asyn_event_queues_manager::remove_event(async_event_param *param) {
    //m_event_fd is the eventfd for event 
    
   // m_epoll_param.revents = 0;
    epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = param->fd;
    if (epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, m_event_fd, &ev) == -1) {
        perror("epoll_ctl");
        return -1;
    }
    return 0;
    
} //remove_event    

//remove file descriptor from event base
int asyn_event_queues_manager::remove_event_base(async_event_param *param) {
    //m_event_base is the event base for event 
    struct event *ev = event_new(m_event_base, param->fd, EV_READ | EV_PERSIST, this->m_read_cb, param);
    if (ev == NULL) {
        perror("event_new");
        return -1;
    }
    if (event_del(ev) == -1) {
        perror("event_del");
        return -1;
    }
    return 0;

} //remove_event_base   
//wait for events on epoll
//wait for events on poll
int asyn_event_queues_manager::wait_poll(async_event_param *param) {
    //m_poll_fd is the eventfd for poll 
    m_poll_param.fd = m_poll_fd;
    m_poll_param.events = POLLIN; 
    pollfd fds[1];
    fds[0].fd = m_poll_fd;
    fds[0].events = POLLIN;
    fds[0].revents = 0;
    if (poll(fds, 1, param->timeout) == -1) {
        perror("poll");
        return -1;
    }
    return 0;
} //wait_poll   
//wait for events on event 
//event queues manager
//event_queues_manager::remove_all

int event_queues_manager::remove_all(async_event_param *param) {
    int ret = 0;
    if (m_epoll_fd > 0) {
        ret = remove_epoll(param);
        if (ret != 0) {
            return ret;
        }
    }
   // if (m_kqueue_fd > 0) {
   //     ret = remove_kqueue(param);
   //     if (ret != 0) {
            return ret;
   //     }
   // }
    if (m_select_fd > 0) {
        ret = remove_select(param);
        if (ret != 0) {
            return ret;
        }
    }
    if (m_poll_fd > 0) {
        ret = remove_poll(param);
        if (ret != 0) {
            return ret;
        }
    }
    if (m_event_fd > 0) {
        ret = remove_event(param);
        if (ret != 0) {
            return ret;
        }
    }
    if (m_event_base) {
        ret = remove_event_base(param);
        if (ret != 0) {
            return ret;
        }
    }
    return 0;

} //remove_all  
//event queues manager
//event_queues_manager::wait_all

int event_queues_manager::wait_all(async_event_param *param) {
    int ret = 0;
    if (m_epoll_fd > 0) {
        ret = wait_epoll(param);
        if (ret != 0) {
            return ret;
        }
    }
    //if (m_kqueue_fd > 0) {
        //ret = wait_kqueue(param);
        //if (ret != 0) {
            //return ret;
        //}
    //}
    if (m_select_fd > 0) {
        ret = wait_select(param);
        if (ret != 0) {
            return ret;
        }
    }
    if (m_poll_fd > 0) {
        ret = wait_poll(param);
        if (ret != 0) {
            return ret;
        }
    }
    if (m_event_fd > 0) {
        ret = wait_event(param);
        if (ret != 0) {
            return ret;
        }
    }
    if (m_event_base) {
        ret = wait_event_base(param);
        if (ret != 0) {
            return ret;
        }
    }
    return 0;

} //wait_all     
//wait for events on epoll
int asyn_event_queues_manager::wait_epoll(async_event_param *param) {
    struct epoll_event events[1];
    if (epoll_wait(m_epoll_fd, events, 1, param->timeout) == -1) {
        perror("epoll_wait");
        return -1;
    }
    return 0;
} //wait_epoll  
//wait for events on select
int asyn_event_queues_manager::wait_select(async_event_param *param) {
    //m_select_fd is the eventfd for select 
    m_poll_param.fd = m_select_fd;
    m_poll_param.events = POLLIN;
    pollfd fds[1];
    fds[0].fd = m_select_fd;
    fds[0].events = POLLIN;
    fds[0].revents = 0;
    if (poll(fds, 1, param->timeout) == -1) {
        perror("poll");
        return -1;
    }
    return 0;
} //wait_select 
//wait for events on poll
  
//wait for events on event
int asyn_event_queues_manager::wait_event(async_event_param *param) {
    //m_event_fd is the eventfd for event 
    eventfd_t value ;

    if (eventfd_read(m_event_fd, &value) == -1) {
        perror("eventfd_read");
        return -1;
    }
    //set the value to the return value 
    param->data = value;
    return 0;
} //wait_event  
//wait for events on event base 
int asyn_event_queues_manager::wait_event_base(async_event_param *param) {
    //m_event_base is the event base for event 
    if (event_base_dispatch(m_event_base) == -1) {
        perror("event_base_dispatch");
        return -1;
    }
    //update the return value
    param->data = 0;

    return 0;
} //wait_event_base 
//get epoll file descriptor
int asyn_event_queues_manager::get_epoll_fd( ) {
    return m_epoll_fd;
} //get_epoll_fd    
//get kqueue file descriptor
//int asyn_event_queues_manager::get_kqueue_fd( ) {
//    return m_kqueue_fd;
//} //get_kqueue_fd
//get select file descriptor
int asyn_event_queues_manager::get_select_fd( ) {
    return m_select_fd;
} //get_select_fd
//get poll file descriptor
int asyn_event_queues_manager::get_poll_fd( ) {
    return m_poll_fd;
} //get_poll_fd
//get event file descriptor
int asyn_event_queues_manager::get_event_fd( ) {
    return m_event_fd;
} //get_event_fd
//get event base
struct event_base *asyn_event_queues_manager::get_event_base( ) {
    return m_event_base;
} //get_event_base
//get signal file descriptor
int asyn_event_queues_manager::get_signal_fd( ) {
    return m_signal_fd;
} //get_signal_fd

//event queues manager
//event_queues_manager
//event_queues_manager::event_queues_manager
event_queues_manager::event_queues_manager( ) {
    m_epoll_fd = -1;
    //m_kqueue_fd = -1;
    m_select_fd = -1;
    m_poll_fd = -1;
    m_event_fd = -1;
    m_signal_fd = -1;
    m_event_base = NULL;    
} //event_queues_manager
//event_queues_manager::~event_queues_manager
event_queues_manager::~event_queues_manager() {
    if (m_epoll_fd > 0) {
        close(m_epoll_fd);
    }
    //if (m_kqueue_fd > 0) {
    //    close(m_kqueue_fd);
    //}
    if (m_select_fd > 0) {
        close(m_select_fd);
    }
    if (m_poll_fd > 0) {
        close(m_poll_fd);
    }
    if (m_event_fd > 0) {
        close(m_event_fd);
    }
    if (m_signal_fd > 0) {
        close(m_signal_fd);
    }
    if (m_event_base) {
        event_base_free(m_event_base);
    }
} //event_queues_manager::~event_queues_manager
//event_queues_manager::init_epoll
int event_queues_manager::init_all( ){
    //init epoll
    int ret = init_epoll();
    if (ret != 0) {
        return ret;
    }
    //init kqueue
    //ret = init_kqueue();
    //if (ret != 0) {
    //    return ret;
    //}
    //init select
    ret = init_select();
    if (ret != 0) {
        return ret;
    }
    //init poll
    ret = init_poll();
    if (ret != 0) {
        return ret;
    }
    //init event
    ret = init_event();
    if (ret != 0) {
        return ret;
    }
    //init event base
    ret = init_event_base();
    if (ret != 0) {
        return ret;
    }
    return 0;
} //init_all
//add all   
int event_queues_manager::add_all(async_event_param *param) {
    int ret = 0;
    //add epoll
    ret = add_epoll(param);
    if (ret != 0) {
        return ret;
    }
    //add kqueue
    //ret = add_kqueue(param);
    //if (ret != 0) {
    //    return ret;
    //}
    //add select
    ret = add_select(param);
    if (ret != 0) {
        return ret;
    }
    //add poll
    ret = add_poll(param);
    if (ret != 0) {
        return ret;
    }
    //add event
    ret = add_event(param);
    if (ret != 0) {
        return ret;
    }
    //add event base
    ret = add_event_base(param);
    if (ret != 0) {
        return ret;
    }
    return 0;
} //add_all 
//remove all
 //add event_base

 //asyn_event_queues_manager::add_event_base

int asyn_event_queues_manager::add_event_base(async_event_param *param) {
    //m_event_base is the event base for event 
    struct event *ev = event_new(m_event_base, param->fd, EV_READ | EV_PERSIST, this->m_read_cb, param);
    if (ev == NULL) {
        perror("event_new");
        return -1;
    }
    if (event_add(ev, NULL) == -1) {
        perror("event_add");
        event_free(ev);
        return -1;
    }
    event_free(ev);
    return 0;   
} //add_event_base
}//namespace happy_proxy