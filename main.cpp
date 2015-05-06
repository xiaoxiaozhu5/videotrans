#include <QtGui/QApplication>
#include "mainwindow.h"
#include "widget.h"
#include "config.h"

#ifdef OK6410
    #include <QWSServer>
#endif

int main(int argc, char *argv[])
{
#ifdef OK6410
    system("ifconfig eth0 down");
    system("ifconfig lo down");
    system("ifconfig wlan0 up");
    system("iwlist wlan0 scan");
    system("iwconfig wlan0 essid \"forlinux\"");
    system("ifconfig wlan0 192.168.1.111");

    QWSServer::setCursorVisible(false);//Hide the cursor
#endif
    QApplication a(argc, argv,QApplication::GuiServer);
    MainWindow m;
    m.setWindowTitle("Video Transmission");
    m.show();
    return a.exec();
}
