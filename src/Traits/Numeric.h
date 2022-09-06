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

#ifndef ODB_TRAITS_NUMERIC
#define ODB_TRAITS_NUMERIC

#include <cstdlib>
#include <odb/pgsql/traits.hxx>

namespace odb {
namespace pgsql {
template<>
///
/// \brief The value_traits<std::size_t, id_numeric> class
/// ODB value_traits specialization to perform std::size_t conversion to/from other PostgreSql numeric type.
/// more: https://www.codesynthesis.com/~boris/blog/2012/07/18/custom-database-to-cxx-type-mapping-in-odb/
///
class value_traits<std::size_t, id_string> {
    using underlying_type = image_traits<id_string>::image_type;

    static constexpr int          DECIMAL_BASE      = 10;
    static constexpr std::size_t  IMAGE_BUFFER_SIZE = 256;

  public:
    using value_type = std::size_t;
    using query_type = value_type;
    using image_type = underlying_type;

    static constexpr inline void  set_value(value_type& value, const image_type& image, std::size_t size, bool isNull) {
        (void)size;

        if (!isNull) {
            value = std::strtoull(image.data(), nullptr, DECIMAL_BASE);
        } else {
            value = std::numeric_limits<value_type>::min();
        }
    }

    static inline void  set_image(image_type& image, std::size_t& size, bool& isNull, const value_type& value) {
        // Underlying PostgreSql NUMERIC buffer is subject to changes so we just serilize to text buffer
        // format: https://doxygen.postgresql.org/backend_2utils_2adt_2numeric_8c_source.html#l01081
        isNull = false;
        std::array<char, IMAGE_BUFFER_SIZE>  buffer;
        size = std::sprintf(buffer.data(), "%zu", value);
        image.capacity(size);
        std::memcpy(image.data(), buffer.data(), size);
    }
};
} // namespace pgsql
} // namespace odb

#endif // ODB_TRAITS_NUMERIC
