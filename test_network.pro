# -------------------------------------------------
# Project created by QtCreator 2015-03-20T06:58:03
# -------------------------------------------------
QT += network widgets
TARGET = test_network
TEMPLATE = app
SOURCES += main.cpp \
    widget.cpp \
    videodevice.cpp \
    camera.cpp \
    udp.cpp \
    mythread.cpp \
    mainwindow.cpp \
    client.cpp
HEADERS += widget.h \
    videodevice.h \
    camera.h \
    udp.h \
    mythread.h \
    config.h \
    mainwindow.h \
    client.h
FORMS += widget.ui \
    camera.ui \
    mainwindow.ui \
    client.ui
