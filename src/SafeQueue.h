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

#ifndef SAFEQUEUE_H
#define SAFEQUEUE_H

#include <QByteArray>
#include <QMutex>
#include <QMutexLocker>
#include <QQueue>

template<typename T>
class SafeQueue {
public:
    int size() {
        QMutexLocker locker(&mutex_);
        return queue_.size();
    }

    bool empty() {
        QMutexLocker locker(&mutex_);
        return queue_.empty();
    }

    void enqueue(const T& data) {
        QMutexLocker locker(&mutex_);
        queue_.enqueue(data);
    }

    T dequeue() {
        QMutexLocker locker(&mutex_);
        return queue_.dequeue();
    }

private:
    QQueue<T> queue_;
    QMutex mutex_;
};

#endif
