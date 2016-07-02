#-------------------------------------------------
#
# Project created by QtCreator 2015-12-11T14:10:49
#
#-------------------------------------------------

QT       += core gui network multimedia widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 0.1.1
QMAKE_TARGET_COMPANY = 4KUA
QMAKE_TARGET_PRODUCT = AppChat
QMAKE_TARGET_DESCRIPTION = Chat
QMAKE_TARGET_COPYRIGHT = Rumiankov Dima

TARGET = QChat2
TEMPLATE = app


SOURCES += main.cpp\
        qchat.cpp \
    ex_qlabel.cpp \
    cryptalgorithm.cpp \
    abstractparser.cpp \
    contactparser.cpp

HEADERS  += qchat.h \
    ex_qlabel.h \
    cryptalgorithm.h \
    abstractparser.h \
    contactparser.h

FORMS    += qchat.ui
