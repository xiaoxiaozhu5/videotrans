#include <QtGui>
#include "videodevice.h"

class Widget *showerr;

VideoDevice::VideoDevice(QString dev_name)
{
    this->dev_name = dev_name;
    this->fd = -1;
    this->buffers = NULL;
    this->n_buffers = 0;
    this->index = -1;

}

int VideoDevice::open_device()
{
    fd = open(dev_name.toStdString().c_str(), O_RDWR/*|O_NONBLOCK*/, 0);
    if(-1 == fd)
    {
        showerr->display_error(tr("open: %1").arg(QString(strerror(errno))));
        return -1;
    }
    return 0;
}

int VideoDevice::close_device()
{
    if(-1 == close(fd))
    {
        showerr->display_error(tr("close: %1").arg(QString(strerror(errno))));
        return -1;
    }
    return 0;
}

int VideoDevice::init_device()
{
    v4l2_capability cap;
    v4l2_cropcap cropcap;
    v4l2_crop crop;
    v4l2_format fmt;

    if(-1 == ioctl(fd, VIDIOC_QUERYCAP, &cap))
    {
        if(EINVAL == errno)
        {
            showerr->display_error(tr("%1 is no V4l2 device").arg(dev_name));
        }
        else
        {
            showerr->display_error(tr("VIDIOC_QUERYCAP: %1").arg(QString(strerror(errno))));
        }
        return -1;
    }

    if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
    {
        showerr->display_error(tr("%1 is no video capture device").arg(dev_name));
        return -1;
    }

    if(!(cap.capabilities & V4L2_CAP_STREAMING))
    {
        showerr->display_error(tr("%1 does not support streaming i/o").arg(dev_name));
        return -1;
    }

    CLEAR(cropcap);

    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if(0 == ioctl(fd, VIDIOC_CROPCAP, &cropcap))
    {
        CLEAR(crop);
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c = cropcap.defrect;

        if(-1 == ioctl(fd, VIDIOC_S_CROP, &crop))
        {
            if(EINVAL == errno)
            {
//                emit display_error(tr("VIDIOC_S_CROP not supported"));
            }
            else
            {
                showerr->display_error(tr("VIDIOC_S_CROP: %1").arg(QString(strerror(errno))));
                return -1;
            }
        }
    }
    else
    {
        showerr->display_error(tr("VIDIOC_CROPCAP: %1").arg(QString(strerror(errno))));
        return -1;
    }

    CLEAR(fmt);

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = 320;
    fmt.fmt.pix.height = 240;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;


    if(-1 == ioctl(fd, VIDIOC_S_FMT, &fmt))
    {
        showerr->display_error(tr("VIDIOC_S_FMT").arg(QString(strerror(errno))));
        return -1;
    }

    if(-1 == init_mmap())
    {
        return -1;
    }

    return 0;
}

int VideoDevice::init_mmap()
{
    struct v4l2_requestbuffers req;
    CLEAR(req);

    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if(-1 == ioctl(fd, VIDIOC_REQBUFS, &req))
    {
        if(EINVAL == errno)
        {
            showerr->display_error(tr("%1 does not support memory mapping").arg(dev_name));
            return -1;
        }
        else
        {
            showerr->display_error(tr("VIDIOC_REQBUFS %1").arg(QString(strerror(errno))));
            return -1;
        }
    }

    if(req.count < 2)
    {
        showerr->display_error(tr("Insufficient buffer memory on %1").arg(dev_name));
        return -1;
    }

    buffers = (buffer*)calloc(req.count, sizeof(*buffers));

    if(!buffers)
    {
        showerr->display_error(tr("out of memory"));
        return -1;
    }

    for(n_buffers = 0; n_buffers < req.count; ++n_buffers)
    {
        struct v4l2_buffer buf;
        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = n_buffers;

        if(-1 == ioctl(fd, VIDIOC_QUERYBUF, &buf))
        {
            showerr->display_error(tr("VIDIOC_QUERYBUF: %1").arg(QString(strerror(errno))));
            return -1;
        }

        buffers[n_buffers].length = buf.length;
        buffers[n_buffers].start =
                mmap(NULL,
                     buf.length,
                     PROT_READ | PROT_WRITE,
                     MAP_SHARED,
                     fd, buf.m.offset);

        if(MAP_FAILED == buffers[n_buffers].start)
        {
            showerr->display_error(tr("mmap %1").arg(QString(strerror(errno))));
            return -1;
        }
    }
    return 0;

}

void VideoDevice::readOneframe()
{
      int ret;
      int frame=0;
      struct video_mmap cam_mm;
      cam_mm.frame = 0;
      cam_mm.format=VIDEO_PALETTE_YUYV;

      ret = ioctl(fd,VIDIOCMCAPTURE,&cam_mm);
      if( ret<0 ) {
        showerr->display_error("ERROR: VIDIOCMCAPTURE");
      }

      ret = ioctl(fd,VIDIOCSYNC,&frame);
      if( ret<0 ) {
        showerr->display_error("ERROR: VIDIOCSYNC");
      }
}

int VideoDevice::start_capturing()
{
    unsigned int i;

    for(i = 0; i < n_buffers; ++i)
    {
        v4l2_buffer buf;
        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory =V4L2_MEMORY_MMAP;
        buf.index = i;

        if(-1 == ioctl(fd, VIDIOC_QBUF, &buf))
        {
            showerr->display_error(tr("VIDIOC_QBUF: %1").arg(QString(strerror(errno))));
            return -1;
        }
    }

    v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if(-1 == ioctl(fd, VIDIOC_STREAMON, &type))
    {
        showerr->display_error(tr("VIDIOC_STREAMON: %1").arg(QString(strerror(errno))));
        return -1;
    }
    return 0;
}

int VideoDevice::stop_capturing()
{
    v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if(-1 == ioctl(fd, VIDIOC_STREAMOFF, &type))
    {
        showerr->display_error(tr("VIDIOC_STREAMOFF: %1").arg(QString(strerror(errno))));
        return -1;
    }
    return 0;
}

int VideoDevice::uninit_device()
{
    unsigned int i;
    for(i = 0; i < n_buffers; ++i)
    {
        if(-1 == munmap(buffers[i].start, buffers[i].length))
        {
            showerr->display_error(tr("munmap: %1").arg(QString(strerror(errno))));
            return -1;
        }

    }
    free(buffers);
    return 0;
}

int VideoDevice::get_frame(void **frame_buf, size_t* len)
{
    v4l2_buffer queue_buf;
    CLEAR(queue_buf);

    queue_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    queue_buf.memory = V4L2_MEMORY_MMAP;

    if(-1 == ioctl(fd, VIDIOC_DQBUF, &queue_buf))
    {
        switch(errno)
        {
        case EAGAIN:
//            perror("dqbuf");
            return -1;
        case EIO:
            return -1 ;
        default:
            showerr->display_error(tr("VIDIOC_DQBUF: %1").arg(QString(strerror(errno))));
            return -1;
        }
    }

    *frame_buf = buffers[queue_buf.index].start;
    *len = buffers[queue_buf.index].length;
    index = queue_buf.index;

    return 0;

}

int VideoDevice::unget_frame()
{
    if(index != -1)
    {
        v4l2_buffer queue_buf;
        CLEAR(queue_buf);

        queue_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        queue_buf.memory = V4L2_MEMORY_MMAP;
        queue_buf.index = index;

        if(-1 == ioctl(fd, VIDIOC_QBUF, &queue_buf))
        {
            showerr->display_error(tr("VIDIOC_QBUF: %1").arg(QString(strerror(errno))));
            return -1;
        }
        return 0;
    }
    return -1;
}

void VideoDevice::sendData()
{
    char* frame;
    size_t frame_len;


    if(-1==open_device())
    {
        exit(EXIT_FAILURE);
    }
    qDebug("init device");
    if(-1==init_device())
    {
        exit(EXIT_FAILURE);
    }
    qDebug("start caping");
    if(-1 == start_capturing())
    {
        exit(EXIT_FAILURE);
    }

    if(-1 == get_frame((void**)&frame, &frame_len))
    {
        exit(EXIT_FAILURE);
    }

    //qDebug("addr: %u, len: %d",(unsigned int)frame, frame_len);

    udp->sendData((char *)frame, frame_len);


    if(-1 == unget_frame())
    {
        exit(EXIT_FAILURE);
    }


}




