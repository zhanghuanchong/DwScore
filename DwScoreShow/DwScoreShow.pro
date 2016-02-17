#-------------------------------------------------
#
# Project created by QtCreator 2010-11-17T16:23:18
#
#-------------------------------------------------

QT       += core gui network phonon xml

TARGET = DwScoreShow
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    dwcountdown.cpp \
    globals.cpp \
    logindialog.cpp \
    welcomewidget.cpp \
    countdownwidget.cpp \
    scorewidget.cpp \
    rankwidget.cpp

HEADERS  += widget.h \
    dwcountdown.h \
    globals.h \
    logindialog.h \
    welcomewidget.h \
    countdownwidget.h \
    scorewidget.h \
    rankwidget.h

RC_FILE += resources.rc
