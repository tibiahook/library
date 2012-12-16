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

#include "PacketReader.h"
#include "ProxyInterface.h"
#include "ProxyManager.h"

bool ProxyManager::handleOutgoingPacket(const QByteArray& data) const {
    if (data.length() <= 0) {
        return true;
    }

    ProxyList proxies = outgoingProxies_[(quint8) data[0]];
    if (proxies.length() == 0) {
        return true;
    }

    // Iterate all proxies and stop when one returns false
    PacketReader reader(data);
    foreach (ProxyInterface* proxy, proxies) {
        if (!proxy->handlePacket(reader)) {
            return false;
        }

        // Reset reader position after each handlePacket call
        reader.setPosition(0);
    }
    return true;
}

bool ProxyManager::handleIncomingPacket(const QByteArray& data) const {
    if (data.length() <= 0) {
        return true;
    }

    ProxyList proxies = incomingProxies_[(quint8) data[0]];
    if (proxies.length() == 0) {
        return true;
    }

    // Iterate read-only proxies
    PacketReader reader(data);
    foreach (ProxyInterface* proxy, proxies) {
        if (!proxy->handlePacket(reader)) {
            return false;
        }

        // Reset reader position after each handlePacket call
        reader.setPosition(0);
    }
    return true;
}

void ProxyManager::addOutgoingProxy(quint8 type, ProxyInterface* proxy) {
    outgoingProxies_[type].append(proxy);
}

void ProxyManager::removeOutgoingProxy(quint8 type, ProxyInterface* proxy) {
    outgoingProxies_[type].removeAll(proxy);
}

void ProxyManager::addIncomingProxy(quint8 type, ProxyInterface* proxy) {
    incomingProxies_[type].append(proxy);
}

void ProxyManager::removeIncomingProxy(quint8 type, ProxyInterface* proxy) {
    incomingProxies_[type].removeAll(proxy);
}
