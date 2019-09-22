#-------------------------------------------------
#
# Project created by QtCreator 2018-05-03T17:50:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TradeEmulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dataecsv.cpp \
    funcecsv.cpp \
    funcsmart.cpp \
    iobjectecsv.cpp \
    pipefile.cpp \
    qfuncsmart.cpp \
    structecsv.cpp \
    neuroproject.cpp \
    tradeparametrs.cpp \
    smarttimer.cpp \
    pipemessage.cpp

HEADERS  += mainwindow.h \
    dataecsv.h \
    funcecsv.h \
    funcsmart.h \
    iobjectecsv.h \
    narray.h \
    nmatrix.h \
    nmatrixecsv.h \
    pipefile.h \
    qfuncsmart.h \
    structecsv.h \
    neuroproject.h \
    tradeparametrs.h \
    smarttimer.h \
    defsmart.h \
    pipemessage.h

FORMS    += mainwindow.ui
