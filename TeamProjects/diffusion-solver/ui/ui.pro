#-------------------------------------------------
#
# Project created by QtCreator 2014-12-22T16:37:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = ui
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        dsmainwindow.cpp \
    dsmodel.cpp \
    ext/qcustomplot.cpp

HEADERS  += dsmainwindow.hpp \
    dsmodel.hpp \
    ext/qcustomplot.h

FORMS    += dsmainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../core-build/release/ -ldiffusion-solver
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../core-build/debug/ -ldiffusion-solver
else:unix:!macx: LIBS += -L$$PWD/../core-build/ -ldiffusion-solver

INCLUDEPATH += $$PWD/../core/include
DEPENDPATH += $$PWD/../core/include
