/********************************************************************************
** Form generated from reading UI file 'camera.ui'
**
** Created: Wed May 6 04:51:55 2015
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMERA_H
#define UI_CAMERA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_camera
{
public:
    QPushButton *pushButton;
    QLabel *label;

    void setupUi(QWidget *camera)
    {
        if (camera->objectName().isEmpty())
            camera->setObjectName(QString::fromUtf8("camera"));
        camera->resize(480, 272);
        pushButton = new QPushButton(camera);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(360, 110, 91, 51));
        label = new QLabel(camera);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(21, 17, 321, 241));

        retranslateUi(camera);
        QObject::connect(pushButton, SIGNAL(clicked()), camera, SLOT(back2main()));

        QMetaObject::connectSlotsByName(camera);
    } // setupUi

    void retranslateUi(QWidget *camera)
    {
        camera->setWindowTitle(QApplication::translate("camera", "Form", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("camera", "&Close", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class camera: public Ui_camera {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMERA_H
