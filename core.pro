QMAKE_CXXFLAGS += -std=c++0x

QT += core gui

TEMPLATE = lib
CONFIG += dll

TARGET = tibia-hook

INCLUDEPATH += include $${LIB_MOLOGIE_DETOURS_SRC_DIR}

SOURCES += \
    src/Application.cpp \
    src/DetourManager.cpp \
    src/UIManager.cpp \
    src/PluginManager.cpp \
    src/ProxyManager.cpp \
    src/JsonSettings.cpp \
    src/UILogger.cpp

HEADERS += \
    src/Application.h \
    src/DataQueue.h \
    src/DetourManager.h \
    src/DetourSender.h \
    src/UIManager.h \
    src/PluginManager.h \
    src/ProxyManager.h \
    src/Memory.h \
    src/JsonSettings.h \
    src/Logger.h \
    src/UILogger.h \
    include/UIManagerInterface.h \
    include/SettingsInterface.h \
    include/Serialization.h \
    include/SenderInterface.h \
    include/ProxyInterface.h \
    include/Position.h \
    include/PluginManagerInterface.h \
    include/PluginLoader.h \
    include/PluginInterface.h \
    include/PacketReader.h \
    include/PacketCodes.h \
    include/PacketBuilder.h \
    include/Packet.h \
    include/MemoryInterface.h \
    include/HookInterface.h \
    include/Direction.h \
    include/Constants.h \
    include/ProxyManagerInterface.h \
    include/LoggerInterface.h \
    include/RuntimeError.h \
    include/ConcurrentProxy.h

unix:SOURCES += \
    src/UnixEntry.cpp \
    src/UnixMemory.cpp

win32:SOURCES += \
    src/WindowsEntry.cpp \
    src/WindowsMemory.cpp

FORMS += \
    src/UILogger.ui

OTHER_FILES += \
    files/config.json

include(../mologie-detours/mologie-detours.pri)
include(../qt-json/qt-json.pri)

include(../shared.pri)

settings.path = $${DESTDIR}
settings.files = $${OTHER_FILES}

INSTALLS += settings
