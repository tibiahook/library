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

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QtGlobal>

enum TalkMode {
    TalkModeNone = 0,
    TalkModeSay = 1,
    TalkModeWhisper = 2,
    TalkModeYell = 3,
    TalkModePrivateFrom = 4,
    TalkModePrivateTo = 5,
    TalkModeChannelManagement = 6,
    TalkModeChannel = 7,
    TalkModeChannelHighlight = 8,
    TalkModeSpell = 9,
    TalkModeNpcFrom = 10,
    TalkModeNpcTo = 11,
    TalkModeGamemasterBroadcast = 12,
    TalkModeGamemasterChannel = 13,
    TalkModeGamemasterPrivateFrom = 14,
    TalkModeGamemasterPrivateTo = 15,
    TalkModeLogin = 16,
    TalkModeWarning = 17,
    TalkModeGame = 18,
    TalkModeFailure = 19,
    TalkModeLook = 20,
    TalkModeDamageDealed = 21,
    TalkModeDamageReceived = 22,
    TalkModeHeal = 23,
    TalkModeExp = 24,
    TalkModeDamageOthers = 25,
    TalkModeHealOthers = 26,
    TalkModeExpOthers = 27,
    TalkModeStatus = 28,
    TalkModeLoot = 29,
    TalkModeTradeNpc = 30,
    TalkModeGuild = 31,
    TalkModePartyManagement = 32,
    TalkModeParty = 33,
    TalkModeBarkLow = 34,
    TalkModeBarkLoud = 35,
    TalkModeReport = 36,
    TalkModeHotkeyUse = 37,
    TalkModeTutorialHint = 38,
    TalkModeThankyou = 39,
    TalkModeMarket = 40,
    TalkModeBeyondLast = 41,

    // Deprecated
    TalkModeMonsterYell = 42,
    TalkModeMonsterSay = 43,
    TalkModeRed = 44,
    TalkModeBlue = 45,
    TalkModeRVRChannel = 46,
    TalkModeRVRAnswer = 47,
    TalkModeRVRContinue = 48,
    TalkModeMessageLast = 49,
    TalkModeInvalid = 255
};

#endif
