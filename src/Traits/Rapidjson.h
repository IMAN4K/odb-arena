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

#ifndef ODB_TRAITS_RAPIDJSON
#define ODB_TRAITS_RAPIDJSON

#include <odb/core.hxx>
#include <odb/pgsql/traits.hxx>
#include <rapidjson/allocators.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

namespace odb {
namespace pgsql {
using JSON = rapidjson::Document;

template<>
///
/// \brief The value_traits<rapidjson::Document, id_string>
/// ODB value_traits specialization to perform rapidjson::Document conversion to/from other PostgreSql TEXT type.
/// more: https://www.codesynthesis.com/~boris/blog/2012/07/18/custom-database-to-cxx-type-mapping-in-odb/
///
class value_traits<JSON, id_string> {
    using underlying_type = image_traits<id_string>::image_type;

  public:
    using image_type = underlying_type;
    using value_type = JSON;
    using query_type = value_type;

    static inline void  set_value(value_type& value, const image_type& image, std::size_t size, bool isNull) {
        if (isNull) {
            value.SetObject();
        } else {
            value.Parse(image.data(), size);
        }
    }

    static inline void  set_image(image_type& image, std::size_t& size, bool& isNull, const value_type& value) {
        rapidjson::StringBuffer                     buffer;
        rapidjson::Writer<rapidjson::StringBuffer>  writer(buffer);

        isNull = !value.Accept(writer);

        if (!isNull) {
            size = buffer.GetSize();
            image.capacity(size);
            std::memcpy(image.data(), buffer.GetString(), size);
        }
    }
};
} // namespace pgsql
} // namespace odb

#endif // ODB_TRAITS_RAPIDJSON
