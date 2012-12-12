QT += core gui

TEMPLATE = lib
TARGET = tibia-hook

DESTDIR = bin
OBJECTS_DIR = $${DESTDIR}/.obj
MOC_DIR = $${DESTDIR}/.moc
RCC_DIR = $${DESTDIR}/.rcc
UI_DIR = $${DESTDIR}/.ui

win32 {
    CONFIG += exceptions dll
    DEFINES += WIN32
    DEFINES -= UNICODE
    QMAKE_CXXFLAGS += -U__STRICT_ANSI__
}

QMAKE_CXXFLAGS_WARN_OFF += -Wmissing-field-initializers

LIBS += \
    -Llib/mologie-detours/bin \
    -Llib/qt-json/bin \
    -lmologie-detours \
    -lqt-json

INCLUDEPATH += \
    include \
    lib/mologie-detours/src \
    lib/qt-json/src

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
    src/ProxyReceiver.h \
    src/JsonSettings.h \
    src/Logger.h \
    src/UILogger.h \
    include/UIManagerInterface.h \
    include/SettingsInterface.h \
    include/Serialization.h \
    include/SenderInterface.h \
    include/ReceiverInterface.h \
    include/ReadOnlyProxyInterface.h \
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
    include/LoggerInterface.h

unix:SOURCES += \
    src/UnixEntry.cpp \
    src/UnixMemory.cpp

win32:SOURCES += \
    src/WindowsEntry.cpp \
    src/WindowsMemory.cpp

OTHER_FILES += files/config.js

settings.path = $${DESTDIR}
settings.files = $${OTHER_FILES}

INSTALLS += settings

FORMS += \
    src/UILogger.ui
