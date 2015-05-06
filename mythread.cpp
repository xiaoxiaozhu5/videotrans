#include "mythread.h"

MyThread::MyThread()
{
    runing = 1;
    begin = 1;

#ifdef OK6410
    vd = new VideoDevice(tr("/dev/video2"));
#else
    vd = new VideoDevice(tr("/dev/video0"));
#endif

    if(-1 == vd->open_device())
    {
        exit(EXIT_FAILURE);
    }

    if(-1 == vd->init_device())
    {
        exit(EXIT_FAILURE);
    }

    if(-1 == vd->start_capturing())
    {
        exit(EXIT_FAILURE);
    }
}

MyThread::~MyThread()
{
    if(-1 == vd->stop_capturing())
        {
            exit(EXIT_FAILURE);
        }
    if(-1 == vd->uninit_device())
        {
        exit(EXIT_FAILURE);
        }
    if(-1 == vd->close_device())
        {
            exit(EXIT_FAILURE);
        }
}

void MyThread::run()
{
    udp = new Udp();
    char* frame;
    size_t frame_len;

    while(runing && begin)
    {
        if(-1 == vd->get_frame((void**)&frame, &frame_len))
        {
            exit(EXIT_FAILURE);
        }


        udp->sendData((char *)frame, frame_len);


        if(-1 == vd->unget_frame())
        {
            exit(EXIT_FAILURE);
        }
    }

}

void MyThread::stop()
{
    runing = 0;
}
