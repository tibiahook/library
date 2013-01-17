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

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

#include <TibiaHook/Bot.h>
#include <TibiaHook/BinaryData.h>

#include "DetourSender.h"
#include "Logger.h"
#include "Memory.h"
#include "PluginManager.h"
#include "ProxyManager.h"
#include "UserInterface.h"

class LoggerWidget;
class UserInterface;

class Application: public QApplication, public TibiaHook::Bot {
public:
    Application();
    ~Application();

    void initialize();

    inline void sendToClient(const QByteArray& data) {
        DetourManager::clientQueue()->enqueue(data);
    }

    inline void sendToClient(const TibiaHook::BinaryData& data) {
        DetourManager::clientQueue()->enqueue(data.data());
    }

    inline void sendToServer(const QByteArray& data) {
        DetourManager::serverQueue()->enqueue(data);
    }

    inline void sendToServer(const TibiaHook::BinaryData& data) {
        DetourManager::serverQueue()->enqueue(data.data());
    }

    TibiaHook::Logger* logger() {
        return &logger_;
    }

    TibiaHook::Memory* memory() {
        return &memory_;
    }

    TibiaHook::PluginManager* pluginManager() {
        return &pluginManager_;
    }

    TibiaHook::ProxyManager* proxyManager() {
        return &proxyManager_;
    }

    TibiaHook::UserInterface* userInterface() {
        return userInterface_;
    }

    static Application* instance() {
        return (Application*) QCoreApplication::instance();
    }

    void outgoingMessageHandler(const QByteArray& data);
    void incomingMessageHandler(const QByteArray& data);

private:
    Application(const Application&);
    Application& operator=(const Application&);

    Logger logger_;
    Memory memory_;
    PluginManager pluginManager_;
    ProxyManager proxyManager_;

    // QObjects need to be heap allocated
    UserInterface* userInterface_;
    LoggerWidget* loggerWidget_;
};

#endif
