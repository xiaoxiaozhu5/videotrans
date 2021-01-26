#ifndef CAMERA_H
#define CAMERA_H

#include <QWidget>
#include "videodevice.h"
#include <QLabel>
#include <QTimer>
#include "config.h"

namespace Ui {
    class camera;
}

class camera : public QWidget {
    Q_OBJECT
public:
    explicit camera(QWidget *parent = Q_NULLPTR);
    ~camera();
    void initialDevice();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::camera *m_ui;
    VideoDevice *vd;
    QImage *frame;
    QTimer *timer;
    int rs;
    uchar *pp;
    uchar * p;
    unsigned int len;
    int convert_yuv_to_rgb_pixel(int y, int u, int v);
    int convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);

private slots:
    void readframe();
    void back2main();
};

#endif // CAMERA_H
