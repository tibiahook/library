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

#ifndef POSITION_H
#define POSITION_H

#include <QMetaType>

#include <Direction.h>

struct Position {
    Position(): x(0), y(0), z(0) {}
    Position(quint16 x_, quint16 y_, quint8 z_): x(x_), y(y_), z(z_) {}
    Position(const Position& other): x(other.x), y(other.y), z(other.z) {}

    quint16 x;
    quint16 y;
    quint8 z;

    Position operator+(const Direction& rhs) const {
        Position moved(x, y, z);

        if ((rhs & DirectionNorth) == DirectionNorth) {
            moved.y -= 1;
        }
        else if ((rhs & DirectionSouth) == DirectionSouth) {
            moved.y += 1;
        }

        if ((rhs & DirectionEast) == DirectionEast) {
            moved.x += 1;
        }
        else if ((rhs & DirectionWest) == DirectionWest) {
            moved.x -= 1;
        }

        if ((rhs & DirectionUp) == DirectionUp) {
            moved.z -= 1;
        }
        else if ((rhs & DirectionDown) == DirectionDown) {
            moved.z += 1;
        }

        return moved;
    }

    bool operator!=(const Position& rhs) const {
        return  x != rhs.x ||
                y != rhs.y ||
                z != rhs.z;
    }

    bool operator==(const Position& rhs) const {
        return  x == rhs.x &&
                y == rhs.y &&
                z == rhs.z;
    }
};

Q_DECLARE_METATYPE(Position)

#endif
