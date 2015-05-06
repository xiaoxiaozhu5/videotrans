/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Wed May 6 07:50:48 2015
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QPushButton *bt_Server;
    QPushButton *bt_Client;

    void setupUi(QWidget *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(480, 272);
        bt_Server = new QPushButton(MainWindow);
        bt_Server->setObjectName(QString::fromUtf8("bt_Server"));
        bt_Server->setGeometry(QRect(270, 90, 121, 91));
        bt_Client = new QPushButton(MainWindow);
        bt_Client->setObjectName(QString::fromUtf8("bt_Client"));
        bt_Client->setGeometry(QRect(80, 90, 121, 91));

        retranslateUi(MainWindow);
        QObject::connect(bt_Server, SIGNAL(clicked()), MainWindow, SLOT(creatServer()));
        QObject::connect(bt_Client, SIGNAL(clicked()), MainWindow, SLOT(creatClient()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Form", 0, QApplication::UnicodeUTF8));
        bt_Server->setText(QApplication::translate("MainWindow", "Server", 0, QApplication::UnicodeUTF8));
        bt_Client->setText(QApplication::translate("MainWindow", "Client", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
