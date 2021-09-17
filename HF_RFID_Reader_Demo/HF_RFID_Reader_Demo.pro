#-------------------------------------------------
#
# Project created by QtCreator 2021-02-19T15:10:43
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HF_RFID_Reader_Demo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        InventoryThread.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        InventoryThread.h \
        mainwindow.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


unix:!macx: LIBS += -L$$PWD/../build-HF_RFID_Reader_Lib-Desktop_Qt_5_13_0_GCC_64bit-Debug/ -lHF_RFID_Reader_Lib

INCLUDEPATH += $$PWD/../HF_RFID_Reader_Lib
DEPENDPATH += $$PWD/../HF_RFID_Reader_Lib
