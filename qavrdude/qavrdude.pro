#-------------------------------------------------
#
# Project created by QtCreator 2009-10-12T12:12:02
#
#-------------------------------------------------

TARGET = qavrdude
TEMPLATE = app
QT += xml sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
RCC_DIR=build
DESTDIR = bin

SOURCES += main.cpp\
    partdeschandler.cpp \
    settings.cpp \
    avrprogrammer.cpp \
    avrpart.cpp \
    mainwindow.cpp \
 #   fusemodel.cpp \
 #   fusedelegate.cpp \
    bitfieldmodel.cpp \
    bitfielddelegate.cpp

HEADERS  += \
    partdeschandler.h \
    settings.h \
    avrprogrammer.h \
    avrpart.h \
    mainwindow.h \
#    dudepartnos.h \
    hexlineedit.h \
#    fusemodel.h \
#    fusedelegate.h \
    bitfieldmodel.h \
    bitfielddelegate.h

FORMS    += \
    mainwindow.ui

RESOURCES += \
    icons/icons.qrc

OTHER_FILES += \
    TODO.txt
