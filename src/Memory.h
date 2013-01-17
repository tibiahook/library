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

#ifndef MEMORY_H
#define MEMORY_H

#include <QtGlobal>
#include <QString>

#include <TibiaHook/Memory.h>

class Memory: public TibiaHook::Memory {
public:
    inline quint8 readUInt8(TibiaHook::Memory::Address address) const {
        return read<quint8>(address);
    }

    inline quint16 readUInt16(TibiaHook::Memory::Address address) const {
        return read<quint16>(address);
    }

    inline quint32 readUInt32(TibiaHook::Memory::Address address) const {
        return read<quint32>(address);
    }

    inline quint64 readUInt64(TibiaHook::Memory::Address address) const {
        return read<quint64>(address);
    }

    inline const char* readRawString(TibiaHook::Memory::Address address) const {
        return (const char*) address;
    }

    inline QString readString(TibiaHook::Memory::Address address) const {
        return QString(readRawString(address));
    }

    TibiaHook::Memory::Address rebase(TibiaHook::Memory::Address address) const;
    static TibiaHook::Memory::Address staticRebase(TibiaHook::Memory::Address address);

private:
    template <typename T>
    inline T read(TibiaHook::Memory::Address address) const {
        return *((T*) address);
    }
};

#endif
