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

#ifndef PROXYMANAGERINTERFACE_H
#define PROXYMANAGERINTERFACE_H

#include <QtGlobal>

class PacketReader;
class ProxyInterface;

class ProxyManagerInterface {
public:
    virtual void addOutgoingProxy(quint8 type, ProxyInterface* proxy) = 0;
    virtual void removeOutgoingProxy(quint8 type, ProxyInterface* proxy) = 0;

    virtual void addIncomingProxy(quint8 type, ProxyInterface* proxy) = 0;
    virtual void removeIncomingProxy(quint8 type, ProxyInterface* proxy) = 0;
};

#endif
