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

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonObject>
#include <QMessageBox>
#include <QPluginLoader>

#include <TibiaHook/Plugin.h>
#include <TibiaHook/RuntimeError.h>

#include "PluginManager.h"

#ifdef Q_OS_WIN
#define PLUGIN_WILDCARD "*.dll"
#else
#define PLUGIN_WILDCARD "*.so"
#endif

PluginInfo* findPluginInfoByName(PluginInfo::List& list, const QString& name, quint16 version) {
    for (PluginInfo::List::iterator it = list.begin(); it != list.end(); ++it) {
        PluginInfo* info = *it;
        if (info->name.compare(name) == 0 && info->version >= version) {
            return info;
        }
    }
    return NULL;
}

PluginManager::PluginManager(TibiaHook::Bot* bot):
    bot_(bot) {
}

PluginManager::~PluginManager() {
    unload();
}

void PluginManager::load(const QList<QString>& pluginDirectories) {
    foreach (const QString& pluginDirectory, pluginDirectories) {
        PluginInfo* info = loadPluginInfo(pluginDirectory);
        if (info != NULL) {
            pluginInfos_.append(info);
            qDebug() << "loaded plugin: " << info->name;
        }
    }

    // Generate dependency graph
    Dependencies dependencies;
    for (PluginInfo::List::iterator it = pluginInfos_.begin(); it != pluginInfos_.end(); ++it) {
        PluginInfo::Dependencies deps = (*it)->dependencies;
        for (PluginInfo::Dependencies::iterator depit = deps.begin(); depit != deps.end(); ++depit) {
            PluginInfo* dependency = findPluginInfoByName(pluginInfos_, depit->first, depit->second);
            if (dependency == NULL) {
                throw TibiaHook::RuntimeError(QString("Could not load '%1': unmet dependency '%2' [%3]!")
                                   .arg((*it)->name)
                                   .arg(depit->first)
                                   .arg(depit->second));
            }
            dependencies.append(Dependency(*it, dependency));
        }
    }

    PluginInfo::List independent(pluginInfos_);
    for (Dependencies::iterator it = dependencies.begin(); it != dependencies.end(); ++it) {
        independent.removeAll(it->first);
    }

    // Topological sort of plugins
    PluginInfo::List sorted;
    while (!independent.empty()) {
        PluginInfo* info = independent.takeFirst();
        sorted.append(info);

        MutableDependencyIterator it(dependencies);
        while (it.hasNext()) {
            // Copy constructor, else dependency.first won't work anymore
            Dependency dependency = it.next();
            if(dependency.second == info) {
                it.remove();
            }

            bool hasOtherDependencies = false;
            for (Dependencies::iterator it = dependencies.begin(); it != dependencies.end(); ++it) {
                if (it->first == dependency.first) {
                    hasOtherDependencies = true;
                    break;
                }
            }

            if (!hasOtherDependencies) {
                independent.append(dependency.first);
            }
        }
    }

    // Sorted state
    pluginInfos_ = sorted;

    // Load the plugins
    for (PluginInfo::List::iterator it = pluginInfos_.begin(); it != pluginInfos_.end(); ++it) {
        PluginInfo* info = *it;
        QObject* instance = info->loader.instance();

        if (instance == 0) {
            qDebug() << "unable to load plugin: " << info->loader.fileName() << info->loader.errorString();
            continue;
        }

        // Check if it is a valid plugin
        TibiaHook::Plugin* plugin = qobject_cast<TibiaHook::Plugin*>(instance);
        if (plugin == 0) {
            qDebug() << "unable to convert plugin: " << info->loader.fileName();
            continue;
        }

        try {
            plugin->install(bot_);
            plugins_.insert(info, instance);
        }
        catch(std::exception& exception) {
            QMessageBox message;
            message.setWindowTitle(QApplication::applicationName());
            message.setText("Could not load \"" + info->name + "\" plugin!");
            message.setDetailedText(exception.what());
            message.setDefaultButton(QMessageBox::Ignore);
            message.exec();
        }
    }
}

void PluginManager::unload() {
    QListIterator<PluginInfo*> it(pluginInfos_);

    // Unload plugins in reverse order
    it.toBack();
    while (it.hasPrevious()) {
        PluginInfo* info = it.previous();
        TibiaHook::Plugin* plugin = qobject_cast<TibiaHook::Plugin*>(plugins_.value(info));

        qDebug() << "uninstalling" << info->name;
        plugin->uninstall();

        delete plugin;
        delete info;
    }
    plugins_.clear();
    pluginInfos_.clear();
}

QObject* PluginManager::findPluginByName(const QString& name) {
    for (PluginMap::iterator it = plugins_.begin(); it != plugins_.end(); ++it) {
        PluginInfo* info = it.key();
        if(info->name.compare(name) == 0) {
            return it.value();
        }
    }
    return NULL;
}

QObject* PluginManager::findPluginByName(const QString& name, quint16 minVersion) {
    for (PluginMap::iterator it = plugins_.begin(); it != plugins_.end(); ++it) {
        PluginInfo* info = it.key();
        if(info->name.compare(name) == 0 && info->version >= minVersion) {
            return it.value();
        }
    }
    return NULL;
}

PluginInfo* PluginManager::loadPluginInfo(const QString& directory) {
    try {
        return new PluginInfo(directory);
    }
    catch(TibiaHook::RuntimeError& error) {
        qWarning() << error.what();
    }
    return NULL;
}

PluginInfo::PluginInfo(const QString& directoryString) {
    QDir directory(directoryString);
    QStringList candidates = directory.entryList(QStringList() << PLUGIN_WILDCARD, QDir::Files | QDir::NoDotAndDotDot);
    if (candidates.length() == 0) {
        throw TibiaHook::RuntimeError(QString("Could not load shared library in '%1'!").arg(directoryString));
    }

    QString libraryPath = directory.absoluteFilePath(candidates.first());
    loader.setFileName(libraryPath);

    QVariantMap meta = loader.metaData().value("MetaData").toObject().toVariantMap();
    name = meta.value("name").toString();
    version = meta.value("version").toString().toUInt();

    QVariantList deps = meta.value("dependencies").toList();
    for (QVariantList::iterator it = deps.begin(); it != deps.end(); ++it) {
        QVariantList dep = it->toList();

        QString name = dep.at(0).toString();
        quint16 version = dep.at(1).toUInt();
        dependencies.append(PluginInfo::Dependency(name, version));
    }
}
