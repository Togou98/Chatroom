#-------------------------------------------------
#
# Project created by QtCreator 2020-02-03T13:52:44
#
#-------------------------------------------------

QT       += core gui
QT       += network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client

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

CONFIG += c++11

SOURCES += \
        main.cpp \
        widget.cpp \
    startup.cpp \
    reg.cpp \
    src/lib_json/json_value.cpp \
    src/lib_json/json_writer.cpp \
    src/lib_json/json_reader.cpp

HEADERS += \
        widget.h \
    startup.h \
    nettools.h \
    reg.h \
    src/lib_json/json_tool.h

FORMS += \
        widget.ui \
    startup.ui \
    reg.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    IMG/res.qrc
RC_ICONS = IMG/ico.ico

