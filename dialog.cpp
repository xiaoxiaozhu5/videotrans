#include <QtGui>
#include "dialog.h"
#include "videodevice.h"
#include "ui_dialog.h"


Dialog::Dialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);
    initialDevice();
//    qDebug("consture done!");
}

Dialog::~Dialog()
{
    delete ui;
    rs = vd->stop_capturing();
    rs = vd->uninit_device();
    rs = vd->close_device();
}


void Dialog::setBtnText(QString string)
{
    flag = 0;
    ui->btn_open->setText(string);
}

void Dialog::initialDevice()
{
    pp = (unsigned char *)malloc(320 * 240 * 3 * sizeof(char));
    painter = new QPainter(this);
    frame = new QImage(pp,320,240,QImage::Format_RGB888);
    label = new QLabel();
    vd = new VideoDevice(tr("/dev/video2"));

    connect(vd, SIGNAL(display_error(QString)), this,SLOT(display_error(QString)));
    rs = vd->open_device();
    if(-1==rs)
    {
        QMessageBox::warning(this,tr("error"),tr("open /dev/dsp error"),QMessageBox::Yes);
        vd->close_device();
    }

    rs = vd->init_device();
    if(-1==rs)
    {
        QMessageBox::warning(this,tr("error"),tr("init failed"),QMessageBox::Yes);
        vd->close_device();
    }

    rs = vd->start_capturing();
    if(-1==rs)
    {
        QMessageBox::warning(this,tr("error"),tr("start capture failed"),QMessageBox::Yes);
        vd->close_device();
    }

    if(-1==rs)
    {
        QMessageBox::warning(this,tr("error"),tr("get frame failed"),QMessageBox::Yes);
        vd->stop_capturing();
    }

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(repaint()));
    timer->start(3000);

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(label);
    setLayout(hLayout);
    qDebug("initial done!");
}

void Dialog::isClicked()
{
    if(!flag)
    {
        setBtnText("&Turn on");
        flag = 1;
//        initialDevice();
//        connect(ui->btn_open,SIGNAL(clicked()),this,SLOT(update()));
    }
    else
    {
        setBtnText("&Turn off");
//       connect(ui->btn_open,SIGNAL(clicked()),this,SLOT(shutdown()));
        shutdown();
    }
}

void Dialog::draw()
{
    rs = vd->get_frame((void **)&p,&len);
    convert_yuv_to_rgb_buffer(p,pp,320,240);
    frame->loadFromData((uchar *)pp,320 * 240 * 3 * sizeof(char));

    painter->begin(this);
 //   painter->drawImage(0,0,*frame);
    label->setPixmap(QPixmap::fromImage(*frame,Qt::AutoColor));
    rs = vd->unget_frame();
    painter->end();

 //   label->setPixmap(QPixmap::fromImage(*frame,Qt::AutoColor));

 //   rs = vd->unget_frame();
}

void Dialog::shutdown()
{
    vd->stop_capturing();
    vd->uninit_device();
    vd->close_device();
}

void Dialog::paintEvent(QPaintEvent *)
{
    draw();
//    rs = vd->get_frame((void **)&p,&len);
//    convert_yuv_to_rgb_buffer(p,pp,320,240);
//    frame->loadFromData((uchar *)pp,320 * 240 * 3 * sizeof(char));

//    label->setPixmap(QPixmap::fromImage(*frame,Qt::AutoColor));

//    rs = vd->unget_frame();
}

void Dialog::display_error(QString err)
{
    QMessageBox::warning(this,tr("error"), err,QMessageBox::Yes);
}

/*yuv格式转换为rgb格式*/
int Dialog::convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height)
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

int Dialog::convert_yuv_to_rgb_pixel(int y, int u, int v)
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
/*yuv格式转换为rgb格式*/

