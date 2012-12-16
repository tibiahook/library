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

#include <HookInterface.h>

#include "Logger.h"
#include "JsonSettings.h"
#include "Memory.h"
#include "PluginManager.h"
#include "ProxyManager.h"
#include "DetourSender.h"
#include "UILogger.h"
#include "UIManager.h"

class Application: public QApplication, public HookInterface {
public:
    Application();
    ~Application();

    void initialize();

    LoggerInterface* logger() { return &logger_; }
    MemoryInterface* memory() { return &memory_; }
    PluginManagerInterface* plugins() { return &plugins_; }
    ProxyManagerInterface* proxies() { return &proxies_; }
    SenderInterface* sender() { return &sender_; }
    SettingsInterface* settings() { return &settings_; }
    UIManagerInterface* ui() { return ui_; }

    static Application* instance() {
        return (Application*) QCoreApplication::instance();
    }

private:
    Application(const Application&);
    Application& operator=(const Application&);

    Logger logger_;
    JsonSettings settings_;
    Memory memory_;
    DetourSender sender_;
    PluginManager plugins_;
    ProxyManager proxies_;

    // QObjects need to be heap allocated
    UIManager* ui_;
    UILogger* uiLogger_;
};

#endif
