#include "mythread.h"

MyThread::MyThread()
{
    runing_ = 1;

#ifdef OK6410
    vd = new VideoDevice(tr("/dev/video2"));
#else
    vd_ = new VideoDevice(tr("/dev/video0"));
#endif

    if(-1 == vd_->open_device())
    {
        exit(EXIT_FAILURE);
    }

    if(-1 == vd_->init_device())
    {
        exit(EXIT_FAILURE);
    }

    if(-1 == vd_->start_capturing())
    {
        exit(EXIT_FAILURE);
    }
}

MyThread::~MyThread()
{
    if(-1 == vd_->stop_capturing())
        {
            exit(EXIT_FAILURE);
        }
    if(-1 == vd_->uninit_device())
        {
        exit(EXIT_FAILURE);
        }
    if(-1 == vd_->close_device())
        {
            exit(EXIT_FAILURE);
        }
}

void MyThread::run()
{
    udp_ = new Udp();
    char* frame;
    size_t frame_len;

    while(runing_)
    {
        if(-1 == vd_->get_frame((void**)&frame, &frame_len))
        {
            exit(EXIT_FAILURE);
        }

        udp_->sendData((char *)frame, frame_len);

        if(-1 == vd_->unget_frame())
        {
            exit(EXIT_FAILURE);
        }
    }

}

void MyThread::stop()
{
    runing_ = 0;
}
