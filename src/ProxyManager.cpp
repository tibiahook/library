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

#include <QDebug>

#include <TibiaHook/BinaryReader.h>
#include <TibiaHook/Proxy.h>

#include "ProxyManager.h"

bool ProxyManager::handleBinaryData(const QByteArray& data, const ProxyList* proxyLists) {
    if (data.length() <= 0) {
        return true;
    }

    ProxyList proxyList = proxyLists[(quint8) data[0]];
    if (proxyList.length() == 0) {
        return true;
    }

    // Iterate all proxies and stop when one returns false
    TibiaHook::BinaryReader reader(data);
    foreach (TibiaHook::Proxy* proxy, proxyList) {
        try {
            if (!proxy->handleMessage(reader)) {
                return false;
            }
        }
        catch (std::exception&) {
            qDebug() << "unhandled exception in proxy";
        }

        // Reset reader position after each handleBinaryData call
        reader.setPosition(0);
    }
    return true;
}

bool ProxyManager::handleOutgoingBinaryData(const QByteArray& data) const {
    return handleBinaryData(data, outgoingProxyLists_);
}

bool ProxyManager::handleIncomingBinaryData(const QByteArray& data) const {
    return handleBinaryData(data, incomingProxyLists_);
}

void ProxyManager::addOutgoingProxy(quint8 type, TibiaHook::Proxy* proxy) {
    outgoingProxyLists_[type].append(proxy);
}

void ProxyManager::removeOutgoingProxy(quint8 type, TibiaHook::Proxy* proxy) {
    outgoingProxyLists_[type].removeAll(proxy);
}

void ProxyManager::addIncomingProxy(quint8 type, TibiaHook::Proxy* proxy) {
    incomingProxyLists_[type].append(proxy);
}

void ProxyManager::removeIncomingProxy(quint8 type, TibiaHook::Proxy* proxy) {
    incomingProxyLists_[type].removeAll(proxy);
}
