#-------------------------------------------------
#
# Project created by QtCreator 2019-12-12T20:10:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Work
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    dijkstra.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11

RESOURCES += \
    picture.qrc \
    qyh.qrc \
    map.qrc

DISTFILES += \
    qyh.jpg


#INCLUDEPATH += D:\QT5.5\build\install\include
#            D:\QT5.5\build\install\include\opencv2

#LIBS +=  D:\QT5.5\build\lib\libopencv_*.a

#INCLUDEPATH += D:\QT5.5\build\OpencvBuildLib\include
#            D:\QT5.5\build\OpencvBuildLib\include\opencv2\

#QMAKE_CFLAGS += -std=c99
#CONFIG   += c++11 (Qt5)

