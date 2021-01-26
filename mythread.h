#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include "videodevice.h"
#include "udp.h"

class VideoDevice;
class Udp;


class MyThread : public QThread
{
    Q_OBJECT
public:
    MyThread();
    ~MyThread();
    void run();
    void stop();
private:
    VideoDevice *vd_;
    Udp *udp_;
    int runing_;
};

#endif // MYTHREAD_H
