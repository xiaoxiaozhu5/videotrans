#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "camera.h"
#include <qdatetime.h>
#include "udp.h"
#include "mythread.h"
#include "config.h"

//const char setNet[4] = {{"ifconfig eth0 down"},{"iwconfig wlan0 mode ad-hoc"},{"iwconfig wlan0 essid \"test\""},{"ifconfig wlan0 192.168.1.111"}};

namespace Ui
{
    class Widget;
}

class Udp;
class MyThread;
class VideoDevice;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    QDateTime *time;
    QString timeStr;

private:
    Ui::Widget *ui;
    int rs;
    int isFirst;
    QTimer *timer;
    Udp *udp;
    void YUV422To420(unsigned char *pYUV, unsigned char*yuv, int lWidth, int lHeight);
public slots:
    void openCamera();
    void display_error(QString err);
private slots:
    void startTCP();

};

#endif // WIDGET_H
