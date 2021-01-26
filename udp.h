#ifndef UDP_H
#define UDP_H

#include <QUdpSocket>
#include <QThread>
#include "widget.h"

#define PAYLOAD_SIZE (30*1024)

class Widget;

typedef struct _DataPacketHeader
{
    quint64 id; //the specific sign
    quint32 total_num;
    quint32 num;
    quint32 len;
} DataPacketHeader;

typedef struct _DataPacket
{
    quint8 payload[PAYLOAD_SIZE];
    struct _DataPacketHeader header;
} DataPacket;

class Udp : public QThread
{
    Q_OBJECT
public:
    explicit Udp(QObject *parent = 0);
    void sendData(char *frame, quint32 frame_len);
    ~Udp();
signals:

private:
    QHostAddress addr;
    quint16 port;
    DataPacket *data_packet;
    QUdpSocket *socket;
    void convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);
    int convert_yuv_to_rgb_pixel(int y, int u, int v);
    Widget *showing;
};

#endif // UDP_H
