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

#ifndef DETOURMANAGER_H
#define DETOURMANAGER_H

#include <QtGlobal>

#include "SafeQueue.h"
#include "Memory.h"

#include <functional>

#ifdef Q_OS_WIN

#include <windows.h>

#define LOOP_FUNCTION_RETURN_TYPE BOOL WINAPI
#define LOOP_FUNCTION_RETURN(value) return value
#define LOOP_FUNCTION_ARG_NAME1 lpMsg
#define LOOP_FUNCTION_ARG_NAME2 hWnd
#define LOOP_FUNCTION_ARG_NAME3 wMsgFilterMin
#define LOOP_FUNCTION_ARG_NAME4 wMsgFilterMax
#define LOOP_FUNCTION_ARG_NAME5 wRemoveMsg
#define LOOP_FUNCTION_ARGUMENTS \
    LOOP_FUNCTION_ARG_NAME1, \
    LOOP_FUNCTION_ARG_NAME2, \
    LOOP_FUNCTION_ARG_NAME3, \
    LOOP_FUNCTION_ARG_NAME4, \
    LOOP_FUNCTION_ARG_NAME5
#define LOOP_FUNCTION_PARAMETERS \
    LPMSG LOOP_FUNCTION_ARG_NAME1, \
    HWND LOOP_FUNCTION_ARG_NAME2, \
    UINT LOOP_FUNCTION_ARG_NAME3, \
    UINT LOOP_FUNCTION_ARG_NAME4, \
    UINT LOOP_FUNCTION_ARG_NAME5
#define LOOP_FUNCTION_PARAMETERS_NO_NAMES LPMSG, HWND,  UINT, UINT,  UINT

#else

#define LOOP_FUNCTION_RETURN_TYPE int
#define LOOP_FUNCTION_RETURN(value) return value
#define LOOP_FUNCTION_ARG_NAME1 display
#define LOOP_FUNCTION_ARGUMENTS LOOP_FUNCTION_ARG_NAME1
#define LOOP_FUNCTION_PARAMETERS void* LOOP_FUNCTION_ARG_NAME1

#endif

namespace MologieDetours {
template <typename function_type> class Detour;
}

class Hook;
class DetourManager {
    typedef LOOP_FUNCTION_RETURN_TYPE LoopSignature(LOOP_FUNCTION_PARAMETERS);

    typedef void OutgoingFunctionSignature(bool);
    typedef void IncomingFunctionSignature();
    typedef int IncomingNextFunctionSignature();

    struct ParseStream {
        quint8* buffer;
        quint32 size;
        quint32 position;
    } ((packed));

public:
    struct Addresses {
        TibiaHook::Memory::Address inFunction;
        TibiaHook::Memory::Address inNextFunction;
        TibiaHook::Memory::Address inStream;

        TibiaHook::Memory::Address outFunction;
        TibiaHook::Memory::Address outBufferLength;
        TibiaHook::Memory::Address outBuffer;
    };

    LOOP_FUNCTION_RETURN_TYPE onLoopTest(LOOP_FUNCTION_PARAMETERS);

    typedef std::function<void (const QByteArray&)> DataHandler;

    static bool install(const Addresses& settings, DataHandler outgoingHandler, DataHandler incomingHandler);
    static void uninstall();

    static inline SafeQueue<QByteArray>* clientQueue() {
        return &clientQueue_;
    }

    static inline SafeQueue<QByteArray>* serverQueue() {
        return &serverQueue_;
    }

private:
    DetourManager();
    ~DetourManager();

    static DataHandler outgoingHandler_;
    static DataHandler incomingHandler_;

    static bool sendingToClient_;
    static SafeQueue<QByteArray> clientQueue_;
    static SafeQueue<QByteArray> serverQueue_;

    static MologieDetours::Detour<LoopSignature*>* loopDetour_;

    static MologieDetours::Detour<IncomingNextFunctionSignature*>* inNextFunctionDetour_;
    static IncomingFunctionSignature* inFunction_;
    static ParseStream* inStream_;

    static MologieDetours::Detour<OutgoingFunctionSignature*>* outFunctionDetour_;
    static quint32* outBufferLength_;
    static quint8* outBufferBinaryDataChecksum_;
    static quint8* outBufferBinaryDataData_;

    static LOOP_FUNCTION_RETURN_TYPE onLoop(LOOP_FUNCTION_PARAMETERS);
    static void onOutgoing(bool);
    static int onIncomingNext();
};

#endif
