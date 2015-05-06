#ifndef DIALOG_H
#define DIALOG_H

#include <QtGui/QDialog>
#include <QLabel>
#include "videodevice.h"

namespace Ui
{
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
    VideoDevice *vd;
    int flag;
    QPainter *painter;
    QLabel *label;
    QImage *frame;
    QTimer *timer;
    int rs;
    uchar *pp;
    uchar * p;
    unsigned int len;
    int convert_yuv_to_rgb_pixel(int y, int u, int v);
    int convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);
    void initialDevice();
private slots:
    void setBtnText(QString string);
    void paintEvent(QPaintEvent *);
    void display_error(QString err);
    void shutdown();
    void isClicked();
    void draw();
};

#endif // DIALOG_H
