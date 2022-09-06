/* MIT License
*
* Copyright (c) 2022 Iman Ahmadvand
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#ifndef ODB_DATA_PRAGMA
#define ODB_DATA_PRAGMA

#include <chrono>
#include <odb/core.hxx>

PRAGMA_DB(map type("NUMERIC") as("TEXT") to("(?)::NUMERIC") from("(?)::TEXT"))
PRAGMA_DB(map type("JSONB") as("TEXT") to("(?)::JSONB") from("(?)::TEXT"))
PRAGMA_DB(value(std::chrono::milliseconds) type("BIGINT"))

PRAGMA_DB(object(Student) table("Students"))

PRAGMA_DB(member(Student::uid) type("UUID") id)
PRAGMA_DB(member(Student::id) type("NUMERIC") not_null)
PRAGMA_DB(member(Student::name) type("TEXT") not_null)
PRAGMA_DB(member(Student::age) type("BIGINT") not_null)
PRAGMA_DB(member(Student::gender) type("BOOL") not_null)
PRAGMA_DB(member(Student::score) type("DOUBLE PRECISION") not_null)
PRAGMA_DB(member(Student::lastUpdate) type("BIGINT") not_null)
PRAGMA_DB(member(Student::data) type("JSONB"))

PRAGMA_DB(index(Student::"Students_uid_name_idx") members(uid, name) method("BTREE"))

#endif // ODB_DATA_PRAGMA
