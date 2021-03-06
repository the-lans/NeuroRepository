#-------------------------------------------------
#
# Project created by QtCreator 2017-03-21T18:04:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NeuroProject
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    narray.cpp \
    nmatrix.cpp \
    neuroproject.cpp \
    inlayer.cpp \
    nlayertanh.cpp \
    nlayersoftsign.cpp \
    nexample.cpp \
    nlayerarctg.cpp \
    ineuronet.cpp \
    ntradetg.cpp \
    datacsvtrade.cpp \
    naprox.cpp \
    idatacsv.cpp \
    datacsvaprox.cpp \
    nlayerlinear.cpp \
    trainbp.cpp

HEADERS  += mainwindow.h \
    narray.h \
    nmatrix.h \
    inlayer.h \
    nlayertanh.h \
    nlayersoftsign.h \
    nexample.h \
    nlayerarctg.h \
    ineuronet.h \
    ntradetg.h \
    datacsvtrade.h \
    neuroproject.h \
    naprox.h \
    idatacsv.h \
    datacsvaprox.h \
    nlayerlinear.h \
    trainbp.h

FORMS    += mainwindow.ui
