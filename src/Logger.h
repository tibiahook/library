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

#ifndef LOGGER_H
#define LOGGER_H

#include <QList>
#include <QMetaType>
#include <QObject>

#include <TibiaHook/Logger.h>

class Logger: public QObject, public TibiaHook::Logger {
    Q_OBJECT

public:
    struct Entry {
        Level level;
        QString tag;
        QString message;
    };

    Logger() {}

    int size() const {
        return entries_.size();
    }

    const Entry* entryAt(int index) const {
        return entries_[index];
    }

    void log(Level level, const QString& tag, const QString& message) {
        Entry* entry = new Entry();
        entry->level = level;
        entry->tag = tag;
        entry->message = message;
        entries_.append(entry);

        emit entryAdded();
    }

    void d(const QString& tag, const QString& message) { log(Debug, tag, message); }
    void e(const QString& tag, const QString& message) { log(Error, tag, message); }
    void i(const QString& tag, const QString& message) { log(Info, tag, message); }
    void v(const QString& tag, const QString& message) { log(Verbose, tag, message); }
    void w(const QString& tag, const QString& message) { log(Warn, tag, message); }

signals:
    void entryAdded();

private:
    Logger(const Logger&);
    Logger& operator=(const Logger&);

    QList<Entry*> entries_;
};

Q_DECLARE_METATYPE(Logger::Entry)

#endif
