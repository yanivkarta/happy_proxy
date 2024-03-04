
#include "aio/aio_control.h" 

//implementation of aio control operations
int aio_control_init(aio_control_t *aio_ctrl, int aio_fildes, off_t aio_offset, volatile void *aio_buf, size_t aio_nbytes, int aio_reqprio, struct sigevent aio_sigevent, int aio_lio_opcode)
{
    aio_ctrl->aio_fildes = aio_fildes;
    aio_ctrl->aio_offset = aio_offset;
    aio_ctrl->aio_buf = aio_buf;
    aio_ctrl->aio_nbytes = aio_nbytes;
    aio_ctrl->aio_reqprio = aio_reqprio;
    aio_ctrl->aio_sigevent = aio_sigevent;
    aio_ctrl->aio_lio_opcode = aio_lio_opcode;
    return 0;
}   

int aio_control_read(aio_control_t *aio_ctrl)
{
    return aio_read(aio_ctrl->aio_cb );

}

int aio_control_write(aio_control_t *aio_ctrl)
{
    return aio_write(aio_ctrl->aio_cb ); 
}

int aio_control_error(aio_control_t *aio_ctrl)
{
    return aio_error(aio_ctrl->aio_cb );
}

int aio_control_suspend(aio_control_t *aio_ctrl)
{
    struct timespec timeout = {0, 0};

    return aio_suspend( (const aiocb* const*)&aio_ctrl->aio_cb,  1, &timeout); 

}

int aio_control_cancel(aio_control_t *aio_ctrl)
{
    return aio_cancel( aio_ctrl->aio_fildes, aio_ctrl->aio_cb); 

}
 
// Path: src/util/aio_control.cpp
