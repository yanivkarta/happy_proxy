#ifndef __AIO_CONTROL_H_
#define __AIO_CONTROL_H_


#include <stdint.h>
#include <aio.h>
#include <signal.h>
#include <sys/types.h>

#ifdef __CPLUSPLUS__
extern "C" {
#endif // __CPLUSPLUS__

typedef struct aio_control
{
    int aio_fildes;
    off_t aio_offset;
    volatile void *aio_buf;
    size_t aio_nbytes;
    int aio_reqprio;
    struct sigevent aio_sigevent;
    int aio_lio_opcode;
    struct aiocb* aio_cb ; //read, write, error, suspend, cancel, destroy 
    int aio_ret;
} aio_control_t;    

//aio control operations
int aio_control_init(aio_control_t *aio_ctrl, int aio_fildes, off_t aio_offset, volatile void *aio_buf, size_t aio_nbytes, int aio_reqprio, struct sigevent aio_sigevent, int aio_lio_opcode); 
int aio_control_read(aio_control_t *aio_ctrl);
int aio_control_write(aio_control_t *aio_ctrl);
int aio_control_error(aio_control_t *aio_ctrl);
int aio_control_suspend(aio_control_t *aio_ctrl);
int aio_control_cancel(aio_control_t *aio_ctrl);
int aio_control_destroy(aio_control_t *aio_ctrl);

#ifdef __CPLUSPLUS__

}

#endif // __CPLUSPLUS__


#endif // __AIO_CONTROL_H_