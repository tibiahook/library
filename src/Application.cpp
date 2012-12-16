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

#include <QMessageBox>

#include <Position.h>
#include <RuntimeError.h>

#include "Application.h"
#include "DetourManager.h"

static int argc_ = 0;

Application::Application():
    QApplication(argc_, NULL),
    settings_(),
    memory_(),
    sender_(),
    plugins_(this),
    proxies_() {

    QApplication::setApplicationName("Tibia Hook");
    QApplication::setApplicationVersion("beta");
    QApplication::setQuitOnLastWindowClosed(false);

    qRegisterMetaType<Position>("Position");
}

void Application::initialize() {
    try {
        // Try to load the configuration file
        QFile configFile(SETTINGS_FILE);
        if (!configFile.open(QFile::ReadOnly)) {
            throw RuntimeError("Could not open '" SETTINGS_FILE "'!");
        }

        // Try to parse the configuration file
        if (!settings_.parse(configFile.readAll())) {
            throw RuntimeError("Could not load '" SETTINGS_FILE "'!");
        }

        QVariant versionValue = settings_.value(SETTINGS_VERSION);
        if (versionValue.type() != QVariant::String) {
            throw RuntimeError("Could not load version!");
        }

        QString version = versionValue.toString();
        QString addressesKey = QString(SETTINGS_ADDRESSES) + ":" + version;
        QVariant addressesValue = settings_.value(addressesKey);
        if (addressesValue.type() != QVariant::Map) {
            throw RuntimeError("Could not load addresses!");
        }

        QVariantMap addressSettings = addressesValue.toMap();
        DetourManager::Addresses addresses;
        addresses.inFunction = addressSettings.value(SETTINGS_ADDRESSES_IN_FUNCTION).toUInt();
        addresses.inNextFunction = addressSettings.value(SETTINGS_ADDRESSES_IN_NEXT_FUNCTION).toUInt();
        addresses.inStream = addressSettings.value(SETTINGS_ADDRESSES_IN_STREAM).toUInt();
        addresses.outFunction = addressSettings.value(SETTINGS_ADDRESSES_OUT_FUNCTION).toUInt();
        addresses.outBufferLength = addressSettings.value(SETTINGS_ADDRESSES_OUT_BUFFER_LENGTH).toUInt();
        addresses.outBuffer = addressSettings.value(SETTINGS_ADDRESSES_OUT_BUFFER).toUInt();

        // Connect the DetourManager with the sender and receiver and install detours
        DetourManager::setClientDataHandler([&proxies_, &sender_] (const QByteArray& data) {
            if (proxies_.handleOutgoingPacket(data)) {
                sender_.sendToServer(data);
            }
        });

        DetourManager::setServerDataHandler([&proxies_] (const QByteArray& data) {
            proxies_.handleIncomingPacket(data);
        });

        DetourManager::install(addresses);

        // Create user interface
        ui_ = new UIManager();
        uiLogger_ = new UILogger(&logger_);
        ui_->addTab(uiLogger_, "Log");

        // Load the plugin directories
        QVariant pluginValue = settings_.value(SETTINGS_PLUGIN_DIRECTORIES);
        if (pluginValue.type() != QVariant::List) {
            throw RuntimeError("Could not load plugin directories!");
        }

        QVariantList pluginDirectories = pluginValue.toList();
        QStringList pluginStrings;
        foreach (const QVariant& pluginDirectory, pluginDirectories) {
            pluginStrings += pluginDirectory.toString();
        }

        // Load plugins from the given plugins directory
        plugins_.load(pluginStrings);
    }
    catch (RuntimeError& exception) {
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
    plugins_.unload();

    // Then delete the user interface
    delete uiLogger_;
    delete ui_;

    // And finally unload detours
    DetourManager::uninstall();
    DetourManager::setServerDataHandler(NULL);
    DetourManager::setClientDataHandler(NULL);
}
