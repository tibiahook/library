TARGET = tibia-hook

QT += core gui widgets
TEMPLATE = lib
CONFIG += c++11 dll

win32 {
    CONFIG += exceptions rtti
    DEFINES += WIN32
    DEFINES -= UNICODE
    QMAKE_CXXFLAGS += -U__STRICT_ANSI__
}

OUTPUTDIR = $$PWD

unix:DESTDIR=$${OUTPUTDIR}/build-unix/$${TARGET}
win32:DESTDIR=$${OUTPUTDIR}/build-win32/$${TARGET}

OBJECTS_DIR = $${DESTDIR}/obj
MOC_DIR = $${DESTDIR}/moc
RCC_DIR = $${DESTDIR}/rcc
UI_DIR = $${DESTDIR}/ui

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

settings.path = $${DESTDIR}
settings.files = $${OTHER_FILES}

INSTALLS += settings
