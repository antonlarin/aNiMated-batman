#-------------------------------------------------
#
# Project created by QtCreator 2014-12-22T16:37:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = ui
TEMPLATE = app

gcc: QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        dsmainwindow.cpp \
    dsmodel.cpp \
    ext/qcustomplot.cpp \
    dsinitconditionsdialog.cpp \
    dssolvingprogressdialog.cpp \
    dssolverthread.cpp \
    dslayerpairanalysiswindow.cpp \
    dsparameterset.cpp \
    dswindowmanager.cpp \
    idswindow.cpp \
    dssummarydialog.cpp \
    dssolverexception.cpp

HEADERS  += dsmainwindow.hpp \
    dsmodel.hpp \
    ext/qcustomplot.h \
    dsinitconditionsdialog.hpp \
    dssolvingprogressdialog.hpp \
    dssolverthread.hpp \
    dslayerpairanalysiswindow.hpp \
    dsparameterset.hpp \
    dswindowmanager.hpp \
    idswindow.hpp \
    dssummarydialog.hpp \
    dssolverexception.hpp

FORMS    += dsmainwindow.ui \
    dsinitconditionsdialog.ui \
    dssolvingprogressdialog.ui \
    dslayerpairanalysiswindow.ui \
    dssummarydialog.ui

win32: LIBS += -L$$PWD/../core/bin/ -ldiffusion-solver
else:unix:!macx: LIBS += -L$$PWD/../core-build/ \
                         -L$$PWD/../core/bin \
                         -ldiffusion-solver

INCLUDEPATH += $$PWD/../core/include
DEPENDPATH += $$PWD/../core/include

RESOURCES += \
    resources.qrc
