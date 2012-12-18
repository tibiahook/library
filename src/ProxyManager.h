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

#ifndef PROXYMANAGER_H
#define PROXYMANAGER_H

#include <QtGlobal>
#include <QByteArray>
#include <QList>

#include <ProxyManagerInterface.h>

class PacketReader;
class ProxyInterface;

class ProxyManager: public ProxyManagerInterface {
    typedef QList<ProxyInterface*> ProxyList;

public:
    bool handleOutgoingPacket(const QByteArray&) const;
    bool handleIncomingPacket(const QByteArray&) const;

    void addOutgoingProxy(quint8, ProxyInterface*);
    void removeOutgoingProxy(quint8, ProxyInterface*);

    void addIncomingProxy(quint8, ProxyInterface*);
    void removeIncomingProxy(quint8, ProxyInterface*);

private:
    static bool handlePacket(const QByteArray& data, const ProxyList* proxies);

    ProxyList outgoingProxyLists_[256];
    ProxyList incomingProxyLists_[256];
};

#endif
