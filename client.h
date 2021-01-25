#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QUdpSocket>
#include <QImage>
#include "udp.h"

#define PAYLOAD_SIZE (30*1024)
#define PACKET_NUM 7
#define PACKAGE_SIZE (PAYLOAD_SIZE*PACKET_NUM)

namespace Ui {
    class Client;
}

class Client : public QWidget {
    Q_OBJECT
public:
    Client(QWidget *parent = 0);
    ~Client();
    void process(DataPacket *packet);
    void updatePicture();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Client *m_ui;
    QUdpSocket *s;
    DataPacket *packet;
    DataPacketHeader *header;
    char *data;
    char *sign;
    QHostAddress addr;
    quint16 port;
    quint64 current_id;
    QImage *img;

public slots:
    void ready_read();
};

#endif // CLIENT_H
