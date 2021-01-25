#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "widget.h"
#include "client.h"
#include <QFile>

namespace Ui {
    class MainWindow;
}

class Widget;

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *m_ui;
    Widget *server;
    Client *client;

private slots:
    void creatServer();
    void creatClient();
};

#endif // MAINWINDOW_H
