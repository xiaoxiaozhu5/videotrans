#include "camera.h"
#include "ui_camera.h"
#include "widget.h"

Widget *showmess;

void image_cleanup_function(void* p)
{
    qDebug() << "cleanup called\n";
    if(p)
    {
        free(p);
    }
}

camera::camera(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::camera)
{
    m_ui->setupUi(this);
    pp = (unsigned char *)malloc(320 * 240 * 3 * sizeof(char));
    timer = new QTimer(this);
    frame = new QImage(pp,320,240,QImage::Format_RGB888, image_cleanup_function);
    initialDevice();
    connect(timer,SIGNAL(timeout()),this,SLOT(readframe()));
}

camera::~camera()
{
    delete m_ui;
}

void camera::back2main()
{
    timer->stop();
    rs = vd->stop_capturing();
    rs = vd->uninit_device();
    rs = vd->close_device();
    close();
}

void camera::initialDevice()
{
#ifdef OK6410
    vd = new VideoDevice(tr("/dev/video2"));
#else
    vd = new VideoDevice(tr("/dev/video0"));
#endif
    rs = vd->open_device();
    if(-1==rs)
    {
        showmess->display_error(tr("error: %1").arg(QString(strerror(errno))));
        vd->close_device();
    }

    rs = vd->init_device();
    if(-1==rs)
    {
        showmess->display_error(tr("error: %1").arg(QString(strerror(errno))));
        vd->close_device();
    }

    rs = vd->start_capturing();
    if(-1==rs)
    {
        showmess->display_error(tr("error: %1").arg(QString(strerror(errno))));
        vd->close_device();
    }

    if(-1==rs)
    {
        showmess->display_error(tr("error: %1").arg(QString(strerror(errno))));
        vd->stop_capturing();
    }
    timer->start(33);
}

void camera::readframe()
{
    rs = vd->get_frame((void **)&p,&len);
    convert_yuv_to_rgb_buffer(p,pp,320,240);
    frame->loadFromData((uchar *)pp,320 * 240 * 3 * sizeof(char));

    m_ui->label->setPixmap(QPixmap::fromImage(*frame,Qt::AutoColor));

    rs = vd->unget_frame();
}

void camera::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/*yuv格式转换为rgb格式*/
int camera::convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height)
{
 unsigned int in, out = 0;
 unsigned int pixel_16;
 unsigned char pixel_24[3];
 unsigned int pixel32;
 int y0, u, y1, v;
 for(in = 0; in < width * height * 2; in += 4) {
  pixel_16 =
   yuv[in + 3] << 24 |
   yuv[in + 2] << 16 |
   yuv[in + 1] <<  8 |
   yuv[in + 0];
  y0 = (pixel_16 & 0x000000ff);
  u  = (pixel_16 & 0x0000ff00) >>  8;
  y1 = (pixel_16 & 0x00ff0000) >> 16;
  v  = (pixel_16 & 0xff000000) >> 24;
  pixel32 = convert_yuv_to_rgb_pixel(y0, u, v);
  pixel_24[0] = (pixel32 & 0x000000ff);
  pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
  pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
  rgb[out++] = pixel_24[0];
  rgb[out++] = pixel_24[1];
  rgb[out++] = pixel_24[2];
  pixel32 = convert_yuv_to_rgb_pixel(y1, u, v);
  pixel_24[0] = (pixel32 & 0x000000ff);
  pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
  pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
  rgb[out++] = pixel_24[0];
  rgb[out++] = pixel_24[1];
  rgb[out++] = pixel_24[2];
 }
 return 0;
}

int camera::convert_yuv_to_rgb_pixel(int y, int u, int v)
{
 unsigned int pixel32 = 0;
 unsigned char *pixel = (unsigned char *)&pixel32;
 int r, g, b;
 r = y + (1.370705 * (v-128));
 g = y - (0.698001 * (v-128)) - (0.337633 * (u-128));
 b = y + (1.732446 * (u-128));
 if(r > 255) r = 255;
 if(g > 255) g = 255;
 if(b > 255) b = 255;
 if(r < 0) r = 0;
 if(g < 0) g = 0;
 if(b < 0) b = 0;
 pixel[0] = r * 220 / 256;
 pixel[1] = g * 220 / 256;
 pixel[2] = b * 220 / 256;
 return pixel32;
}

