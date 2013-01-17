/* Copyright (c) 2012 Gillis Van Ginderachter <supergillis@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <QByteArray>
#include <QJsonDocument>
#include <QMessageBox>

#include <TibiaHook/Position.h>
#include <TibiaHook/RuntimeError.h>

#include "Application.h"
#include "DetourManager.h"
#include "LoggerWidget.h"
#include "UserInterface.h"

#include <functional>

#define SETTINGS_FILE "config.json"

#define SETTINGS_PLUGIN_DIRECTORIES "plugins"
#define SETTINGS_VERSION "version"
#define SETTINGS_ADDRESSES_IN_FUNCTION "inFunction"
#define SETTINGS_ADDRESSES_IN_NEXT_FUNCTION "inNextFunction"
#define SETTINGS_ADDRESSES_IN_STREAM "inStream"
#define SETTINGS_ADDRESSES_OUT_FUNCTION "outFunction"
#define SETTINGS_ADDRESSES_OUT_BUFFER_LENGTH "outBufferLength"
#define SETTINGS_ADDRESSES_OUT_BUFFER "outBuffer"

#ifdef Q_OS_WIN
#define SETTINGS_ADDRESSES "win32:addresses"
#else
#define SETTINGS_ADDRESSES "unix:addresses"
#endif

static int argc_ = 0;

#ifdef LIBRARY_DEBUG
inline void print(const char* description, const QByteArray& array) {
    fprintf(stderr, "%s [", description);
    for (int index = 0; index < array.length(); ++index) {
        fprintf(stderr, "0x%02X", (quint8) array[index]);
        if (index + 1 < array.length()) {
            fprintf(stderr, " ");
        }
    }
    fprintf(stderr, "]\n");
    fflush(stderr);
}
#endif

Application::Application():
    QApplication(argc_, NULL),
    memory_(),
    pluginManager_(this),
    proxyManager_() {

    QApplication::setApplicationName("Tibia Hook");
    QApplication::setApplicationVersion("beta");
    QApplication::setQuitOnLastWindowClosed(false);

    qRegisterMetaType<TibiaHook::Position>("Position");
}

void Application::initialize() {
    try {
        // Try to load the configuration file
        QFile configFile(SETTINGS_FILE);
        if (!configFile.open(QFile::ReadOnly)) {
            throw TibiaHook::RuntimeError("Could not open '" SETTINGS_FILE "'!");
        }

        QJsonDocument document(QJsonDocument::fromJson(configFile.readAll()));
        if (document.isNull()) {
            throw TibiaHook::RuntimeError("Could not load '" SETTINGS_FILE "'!");
        }

        QVariantMap settings = document.toVariant().toMap();
        QVariant addressesValue = settings.value(SETTINGS_ADDRESSES);
        if (addressesValue.type() != QVariant::Map) {
            throw TibiaHook::RuntimeError("Could not load addresses!");
        }

        QVariantMap addressSettings = addressesValue.toMap();
        DetourManager::Addresses addresses;
        addresses.inFunction = addressSettings.value(SETTINGS_ADDRESSES_IN_FUNCTION).toString().toUInt();
        addresses.inNextFunction = addressSettings.value(SETTINGS_ADDRESSES_IN_NEXT_FUNCTION).toString().toUInt();
        addresses.inStream = addressSettings.value(SETTINGS_ADDRESSES_IN_STREAM).toString().toUInt();
        addresses.outFunction = addressSettings.value(SETTINGS_ADDRESSES_OUT_FUNCTION).toString().toUInt();
        addresses.outBufferLength = addressSettings.value(SETTINGS_ADDRESSES_OUT_BUFFER_LENGTH).toString().toUInt();
        addresses.outBuffer = addressSettings.value(SETTINGS_ADDRESSES_OUT_BUFFER).toString().toUInt();

        if (!DetourManager::install(
                    addresses,
                    std::bind(&Application::outgoingMessageHandler, this, std::placeholders::_1),
                    std::bind(&Application::incomingMessageHandler, this, std::placeholders::_1))) {
            throw TibiaHook::RuntimeError("Could not detour functions!");
        }

        // Create user interface
        userInterface_ = new UserInterface();
        loggerWidget_ = new LoggerWidget(&logger_);
        userInterface_->addTab(loggerWidget_, "Log");

        // Load the plugin directories
        QVariant pluginValue = settings.value(SETTINGS_PLUGIN_DIRECTORIES);
        if (pluginValue.type() != QVariant::List) {
            throw TibiaHook::RuntimeError("Could not load plugin directories!");
        }

        QVariantList pluginDirectories = pluginValue.toList();
        QStringList pluginStrings;
        foreach (const QVariant& pluginDirectory, pluginDirectories) {
            pluginStrings += pluginDirectory.toString();
        }

        // Load plugins from the given plugins directory
        pluginManager_.load(pluginStrings);
    }
    catch (TibiaHook::RuntimeError& exception) {
        QMessageBox message;
        message.setWindowTitle(QApplication::applicationName());
        message.setText("Something terrible has happened!");
        message.setDetailedText(exception.what());
        message.setDefaultButton(QMessageBox::Ignore);
        message.exec();
    }
}

Application::~Application() {
    // First unload plugins
    pluginManager_.unload();

    // Then delete the user interface
    delete loggerWidget_;
    delete userInterface_;

    // And finally unload detours
    DetourManager::uninstall();
}

void Application::outgoingMessageHandler(const QByteArray& data) {
#ifdef LIBRARY_DEBUG
    print("outgoing", data);
#endif

    if (proxyManager_.handleOutgoingBinaryData(data)) {
        sendToServer(data);
    }
}

void Application::incomingMessageHandler(const QByteArray& data) {
#ifdef LIBRARY_DEBUG
    print("incoming", data);
#endif

    proxyManager_.handleIncomingBinaryData(data);
}
