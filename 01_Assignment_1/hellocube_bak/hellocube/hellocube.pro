# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = app
TARGET = hellocube
DESTDIR = ../x64/Debug
QT += core widgets gui opengl
CONFIG += debug
DEFINES += WIN64 QT_DLL QT_WIDGETS_LIB
INCLUDEPATH += ../../../../../../../../../Program Files (x86)/Microsoft Visual Studio 14.0/VC/include/GL \
    ./GeneratedFiles \
    . \
    ./GeneratedFiles/Debug
LIBS += -L"../../../../../../../../../Program Files (x86)/Microsoft Visual Studio 14.0/VC/lib"
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(hellocube.pri)