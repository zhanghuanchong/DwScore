#-------------------------------------------------
#
# Project created by QtCreator 2010-11-16T15:46:50
#
#-------------------------------------------------

QT       += core gui network xml sql

TARGET = DwScoreSvr
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp\
        logindialog.cpp \
    initwizard.cpp

HEADERS  += dialog.h\
            logindialog.h \
    initwizard.h

RC_FILE += resources.rc

RESOURCES +=

FORMS += \
    form.ui \
    initwizard.ui
