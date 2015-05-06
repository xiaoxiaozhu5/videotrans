#ifndef VIDEODEVICE_H
#define VIDEODEVICE_H

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>


#include <sys/ioctl.h>
#include <sys/mman.h>

#include <asm/types.h>
#include <linux/videodev2.h>
#include <linux/videodev.h>

#include <QString>
#include <QObject>
#include "widget.h"

#define CLEAR(x) memset(&(x), 0, sizeof(x))

class Udp;

class VideoDevice : public QObject
{
    Q_OBJECT
public:
    VideoDevice(QString dev_name);
    Udp *udp;
    int open_device();
    int close_device();
    int init_device();
    int start_capturing();
    int stop_capturing();
    int uninit_device();
    int get_frame(void **, size_t*);
    int unget_frame();
    void readOneframe();
    void sendData();
private:
    int init_mmap();

    struct buffer
    {
        void * start;
        size_t length;
    };
    QString dev_name;
    int fd;
    buffer* buffers;
    unsigned int n_buffers;
    int index;

};

#endif // VIDEODEVICE_H
