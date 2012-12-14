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
    NONE = 0,

    NORTH = 1,
    EAST  = 2,
    SOUTH = 4,
    WEST = 8,

    UP = 16,
    DOWN = 32,

    NORTHEAST = NORTH | EAST,
    SOUTHEAST = SOUTH | EAST,
    SOUTHWEST = SOUTH | WEST,
    NORTHWEST = NORTH | WEST
};

#endif
