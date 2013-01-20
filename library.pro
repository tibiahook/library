TARGET = tibia-hook

QT += core gui widgets
TEMPLATE = lib
CONFIG += c++11 dll

INCLUDEPATH += include ../api/

DEFINES += LIBRARY_DEBUG

SOURCES += \
    src/Application.cpp \
    src/DetourManager.cpp \
    src/PluginManager.cpp \
    src/ProxyManager.cpp \
    src/EntryPoint.cpp \
    src/Memory.cpp \
    src/UserInterface.cpp \
    src/LoggerWidget.cpp

HEADERS += \
    src/Application.h \
    src/DetourManager.h \
    src/DetourSender.h \
    src/PluginManager.h \
    src/ProxyManager.h \
    src/Memory.h \
    src/Logger.h \
    src/SafeQueue.h \
    src/LoggerWidget.h \
    src/UserInterface.h

FORMS += \
    src/UILogger.ui

OTHER_FILES += \
    files/config.json

include(lib/mologie-detours/mologie-detours.pri)
include(../tibia-hook.pri)
