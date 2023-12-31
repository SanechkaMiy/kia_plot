QT       += core gui printsupport sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17
TARGET = kia_plot
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

# Default rules for deployment.

SOURCES += \
    kia_db.cpp \
    kia_graph.cpp \
    kia_graph_manager.cpp \
        main.cpp \
        mainwindow.cpp \
    qcustomplot.cpp

HEADERS += \
    kia_db.h \
    kia_graph.h \
    kia_graph_manager.h \
        mainwindow.h \
    qcustomplot.h \
    kia_struct.h

FORMS += \
    mainwindow.ui
