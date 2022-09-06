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

#ifndef ODB_DATA
#define ODB_DATA

#include <string>
#include <chrono>
#include <boost/uuid/uuid.hpp>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

enum class Gender : bool {
    Male, Female
};

///
/// \brief The Student struct
///
struct Student final {
    boost::uuids::uuid uid;
    std::size_t id;
    std::string name;
    std::size_t age;
    Gender gender;
    double score;
    std::chrono::milliseconds lastUpdate;
    rapidjson::Document data;
};

inline std::ostream& operator<<(std::ostream& os, const Student& item) {
    std::string data("{}");

    if (!item.data.IsNull()) {
        rapidjson::StringBuffer buffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        if (item.data.Accept(writer)) {
            data = std::string(buffer.GetString(), buffer.GetSize());
        }
    }

    os << "Student("
       << "uid:" << std::string(reinterpret_cast<const char*>(item.uid.data), reinterpret_cast<const char*>(item.uid.data) + sizeof (item.uid.data)) << ", "
       << "id: " << item.id << ", "
       << "name: " << item.name << ", "
       << "age: " << item.age << ", "
       << "gender: " << (static_cast<bool>(item.gender) ? "Male" : "Female") << ", "
       << "score: " << item.score << ", "
       << "lastUpdate" << item.lastUpdate.count() << ", "
       << "data: " << data << ")"
       << std::endl
       ;
    return os;
}

#endif // ODB_DATA
