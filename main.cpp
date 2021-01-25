#include <QApplication>

#include "mainwindow.h"
#include "widget.h"
#include "config.h"

int main(int argc, char *argv[])
{
#ifdef OK6410
    system("ifconfig eth0 down");
    system("ifconfig lo down");
    system("ifconfig wlan0 up");
    system("iwlist wlan0 scan");
    system("iwconfig wlan0 essid \"forlinux\"");
    system("ifconfig wlan0 192.168.1.111");
#endif
    QApplication a(argc, argv);

    MainWindow m;
    m.setWindowTitle("Video Transmission");
    m.show();
    return a.exec();
}
