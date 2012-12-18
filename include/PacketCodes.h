/* Copyright (c) 2012 Gillis Van Ginderachter <supergillis@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License"),
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef PACKETTYPES_H
#define PACKETTYPES_H

#include <QtGlobal>

enum PacketIn {
    PacketInitialGame = 10,

    PacketInMapFull = 100,
    PacketInMapTopRow = 101,
    PacketInMapRightRow = 102,
    PacketInMapBottomRow = 103,
    PacketInMapLeftRow = 104,

    PacketInFloorChangeUp = 190,
    PacketInFloorChangeDown = 191,

    PacketInSpeak = 170,
    PacketInChannelList = 171,
    PacketInChannelOpen = 172,
    PacketInChannelOpenPrivate = 173,
    PacketInChannelClose = 179,

    PacketInMoveNorth = 101,
    PacketInMoveNorthEast = 106,
    PacketInMoveEast = 102,
    PacketInMoveSouthEast = 107,
    PacketInMoveSouth = 103,
    PacketInMoveSouthWest = 108,
    PacketInMoveWest = 104,
    PacketInMoveNorthWest = 109,
    PacketInMoveCancel = 181,

    PacketOutUseItem = 130
};

#endif
