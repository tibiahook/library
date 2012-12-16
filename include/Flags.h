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

#ifndef FLAGS_H
#define FLAGS_H

#include <QtGlobal>

#define FLAG_INDEX(flag) (1 << (flag + 1))

/**
 * @brief The Flags class uses a quint64 to store the flags. A flag is a number in [0 .. 63].
 */
class Flags {
public:
    inline void clear() {
        flags_ = 0;
    }

    inline void set(quint8 flag) {
        flags_ |= FLAG_INDEX(flag);
    }

    inline bool test(quint8 flag) const {
        return (flags_ & FLAG_INDEX(flag)) == FLAG_INDEX(flag);
    }

private:
    quint64 flags_;
};

#endif
