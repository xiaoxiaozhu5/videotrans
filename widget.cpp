#include "widget.h"
#include "ui_widget.h"
#include "camera.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    timer = new QTimer();
    isFirst = 1;
}

Widget::~Widget()
{
    delete ui;
    delete udp;
}

void Widget::openCamera()
{
    camera *cam = new camera();
    cam->setWindowTitle("camera test");

    cam->show();
}

void Widget::startTCP()
{
    display_error("Opening camera......\tOK");
    MyThread *mythread = new MyThread();

    if(isFirst == 1){
        isFirst = 0;
        ui->btn_start->setText("&Terminate");
        ui->btn_test->setEnabled(false);
        mythread->start();
    }
    else if(isFirst == 0){
        isFirst = 1;
        ui->btn_start->setText("&Start");
        ui->btn_test->setEnabled(true);
        mythread->stop();
    }

//    YUV422To420(frame,converted,320,240);
    display_error("Starting transfering......");
}


//pYUV为422，yuv为420
void Widget::YUV422To420(unsigned char *pYUV, unsigned char*yuv, int lWidth, int lHeight)
{
    int i,j;
    qDebug("begin");
    unsigned char*pY = yuv;
    unsigned char *pU = yuv + lWidth*lHeight;
    unsigned char *pV = pU + (lWidth*lHeight)/4;
    unsigned char *pYUVTemp = pYUV;
    unsigned char *pYUVTempNext = pYUV+lWidth*2;
        for(i=0; i<lHeight; i+=2)
        {
            for(j=0; j<lWidth; j+=2)
            {
                pY[j] = *pYUVTemp++;
                pY[j+lWidth] = *pYUVTempNext++;
                pU[j/2] =(*(pYUVTemp) + *(pYUVTempNext))/2;
                pYUVTemp++;
                pYUVTempNext++;
                pY[j+1] = *pYUVTemp++;
                pY[j+1+lWidth] = *pYUVTempNext++;
                pV[j/2] =(*(pYUVTemp) + *(pYUVTempNext))/2;
                pYUVTemp++;
                pYUVTempNext++;
            }
            pYUVTemp+=lWidth*2;
            pYUVTempNext+=lWidth*2;
            pY+=lWidth*2;
            pU+=lWidth/2;
            pV+=lWidth/2;qDebug("92");
        }
}

void Widget::display_error(QString err)
{
    QTextCursor cursor = ui->te_showinfo->textCursor();
    cursor.movePosition(QTextCursor::End,QTextCursor::MoveAnchor,1);
    ui->te_showinfo->setTextCursor(cursor);
    ui->te_showinfo->setTextColor(QColor("red"));
    timeStr = time->currentDateTime().toString("hh:mm:ss");
    ui->te_showinfo->insertPlainText("---" + timeStr + "---" + "   ");
    ui->te_showinfo->insertPlainText(err + "\n" );
    ui->te_showinfo->setTextColor(QColor("black"));
}
