#include "udp.h"
#include <QDateTime>

Udp::Udp(QObject *parent) :
    QThread(parent)
{

    port = PORT;
    addr = QHostAddress(ADDRESS);
//    addr = QHostAddress::LocalHost;
    data_packet = new DataPacket();
    socket = new QUdpSocket();
}

void Udp::sendData(char *frame, quint32 frame_len)
{
    quint32 frag_size = PAYLOAD_SIZE / 6 * 4;
    quint32 total_num = frame_len / frag_size + ((frame_len % frag_size) ? 0 : 1);
    quint32 left_size = 0;
    quint64 id = QDateTime::currentDateTime().toTime_t();
    for(quint32 i = 0; i < total_num; ++i)
    {
        memset(&data_packet->header, 0, sizeof(DataPacketHeader));
        data_packet->header.id = id;
        data_packet->header.total_num = total_num;
        data_packet->header.num = i;
        if((left_size = frame_len - frag_size * i) >= frag_size)
        {
            data_packet->header.len = PAYLOAD_SIZE;
            convert_yuv_to_rgb_buffer((unsigned char*)(frame + i * frag_size), data_packet->payload, frag_size/2, 1);
        }
        else
        {
            data_packet->header.len = left_size / 4 * 6;
            convert_yuv_to_rgb_buffer((unsigned char *)(frame + i * frag_size), data_packet->payload, left_size /2, 1);
        }
        if(-1 == socket->writeDatagram((char *)data_packet, sizeof(DataPacket), addr, port))
            //qDebug("send error");
            //showing->display_error("send error");
            qDebug()<<"error:"<<QAbstractSocket::SocketError();

        msleep(10);
    }
}

int Udp::convert_yuv_to_rgb_pixel(int y ,int u,int v)
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

void Udp::convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height)
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
}

Udp::~Udp()
{
    delete data_packet;
    delete socket;
}
