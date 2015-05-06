#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::Client)
{
    m_ui->setupUi(this);

    s = new QUdpSocket();
    s->bind(QHostAddress("192.168.1.111"),4567);

    connect(s, SIGNAL(readyRead()), this, SLOT(ready_read()));
    current_id = 0;
    data = new char[PACKAGE_SIZE + sizeof(DataPacketHeader)]; //space for the last header in spite of override
    sign = new char[PACKET_NUM];
    header = new DataPacketHeader();
    img = new QImage((unsigned char *)data, 320, 240, QImage::Format_RGB888);
}

Client::~Client()
{
    delete m_ui;
    delete s;
}

void Client::changeEvent(QEvent *e)
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

void Client::ready_read()
{

    if(s->hasPendingDatagrams())
    {
        quint64 len = s->pendingDatagramSize();
        char *read_addr = data;
        quint16 i;
        for(i = 0; i < PACKET_NUM; ++i)
        {
            if(sign[i] == 1)
            {
                continue;
            }
            read_addr += i * PAYLOAD_SIZE;
            break;
        }

        if(-1 == s->readDatagram(read_addr, len, &addr, &port)) //read packet header information
        {
            qDebug("receive data error");
            return;
        }

        packet = (DataPacket *)read_addr;

        process(packet);

        if(i + 1 >= PACKET_NUM)
        {
            updatePicture();
            memset(sign, 0, PACKET_NUM);
        }
    }
}

void Client::process(DataPacket *packet)
{
    if(current_id < packet->header.id)
    {
        memset(sign, 0, PACKET_NUM);
        current_id = packet->header.id;
    }

    memcpy(header, &packet->header, sizeof(DataPacketHeader));
    if( (((char *)packet - data) / PAYLOAD_SIZE != (signed)packet->header.num) &&
            (sign[packet->header.num] == 0))
    {
        char *dest = data + PAYLOAD_SIZE * packet->header.num;
        memcpy(dest, packet, PAYLOAD_SIZE);
    }

    sign[header->num] = 1;
}

void Client::updatePicture()
{
    if(img->isNull())
        qDebug("img null");
    img->loadFromData((uchar *)data, PACKAGE_SIZE);
    m_ui->label->setPixmap(QPixmap::fromImage(*img));
}
