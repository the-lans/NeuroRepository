#-------------------------------------------------
#
# Project created by QtCreator 2017-03-21T18:04:25
# NeuroProject v3 (Last)
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
    dataecsv.cpp \
    structecsv.cpp \
    funcecsv.cpp \
    neuroproject.cpp \
    nfilesconstruct.cpp \
    iobjectecsv.cpp \
    ntrade.cpp \
    funcsmart.cpp \
    qfuncsmart.cpp \
    pipefile.cpp \
    smarttimer.cpp \
    pipemessage.cpp

HEADERS  += mainwindow.h \
    narray.h \
    nmatrix.h \
    inlayer.h \
    nexample.h \
    ineuronet.h \
    ntradetg.h \
    neuroproject.h \
    trainbp.h \
    structecsv.h \
    iobjectecsv.h \
    dataecsv.h \
    funcecsv.h \
    nexamples.h \
    nmatrixecsv.h \
    noperators.h \
    ntradetgtab.h \
    neurocomitet.h \
    ngroupenergy.h \
    neuroconstructor.h \
    nfilesconstruct.h \
    ntradeprofit.h \
    ntradeclasstab.h \
    ntrade.h \
    funcsmart.h \
    qfuncsmart.h \
    pipefile.h \
    smarttimer.h \
    defsmart.h \
    pipemessage.h \
    nagent.h \
    ncartpole.h \
    inlayerstruct.h \
    ineuronetstruct.h \
    inoptimizer.h \
    noptimizeradam.h

FORMS    += mainwindow.ui
