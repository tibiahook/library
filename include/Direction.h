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

#ifndef DIRECTION_H
#define DIRECTION_H

enum Direction {
    DirectionNone = 0,

    DirectionNorth = 1,
    DirectionEast  = 2,
    DirectionSouth = 4,
    DirectionWest = 8,

    DirectionUp = 16,
    DirectionDown = 32,

    DirectionNorthEast = DirectionNorth | DirectionEast,
    DirectionSouthEast = DirectionSouth | DirectionEast,
    DirectionSouthWest = DirectionSouth | DirectionWest,
    DirectionNorthWest = DirectionNorth | DirectionWest
};

#endif
