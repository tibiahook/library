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

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QDir>
#include <QList>
#include <QObject>
#include <QPair>
#include <QPluginLoader>

#include <TibiaHook/Bot.h>
#include <TibiaHook/PluginManager.h>

struct PluginInfo {
    typedef QList<PluginInfo*> List;
    typedef QMutableListIterator<PluginInfo*> MutableListIterator;

    typedef QPair<QString, quint16> Dependency;
    typedef QList<Dependency> Dependencies;

    PluginInfo(const QString& directoryString);

    QPluginLoader loader;
    QString name;
    quint16 version;

    PluginInfo::Dependencies dependencies;
};

class PluginManager: public TibiaHook::PluginManager {
public:
    typedef QMap<PluginInfo*, QObject*> PluginMap;
    typedef QPair<PluginInfo*, PluginInfo*> Dependency;
    typedef QList<Dependency> Dependencies;
    typedef QMutableListIterator<Dependency> MutableDependencyIterator;

    PluginManager(TibiaHook::Bot*);
    ~PluginManager();

    void load(const QList<QString>& pluginDirectories);
    void unload();

    QObject* findPluginByName(const QString& name);
    QObject* findPluginByName(const QString& name, quint16 minVersion);

private:
    PluginInfo* loadPluginInfo(const QString& pluginDirectory);

    TibiaHook::Bot* bot_;
    QString directory_;

    PluginMap plugins_;
    PluginInfo::List pluginInfos_;
};

#endif
